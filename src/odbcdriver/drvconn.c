/*
 * Copyright <2021> Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 */

#include "drvconn.h"

#include <stdio.h>
#include <stdlib.h>

#include "connection.h"
#include "odbc.h"
#include "misc.h"

#ifndef WIN32
#include <sys/socket.h>
#include <sys/types.h>
#else
#include <winsock2.h>
#endif

#include <string.h>

#ifdef WIN32
#include <windowsx.h>

#include "resource.h"
#include "win_setup.h"
#endif
#include "apifunc.h"
#include "dlg_specific.h"

#ifdef WIN32
INT_PTR CALLBACK dconn_FDriverConnectProc(HWND hdlg, UINT wMsg, WPARAM wParam,
                                          LPARAM lParam);
extern HINSTANCE s_hModule; /* Saved module handle. */
#endif

char *hide_password(const char *str) {
    char *outstr, *pwdp;

    if (!str)
        return NULL;
    outstr = strdup(str);
    if (!outstr)
        return NULL;

    const char *str_to_hide[] = {"PWD="};
    size_t size = sizeof(str_to_hide) / sizeof(str_to_hide[0]);

    for (size_t i = 0; i < size; ++i) {
        if (pwdp = stristr(outstr, str_to_hide[i]), pwdp) {
            char *p;
            for (p = pwdp + strlen(str_to_hide[i]); *p && *p != ';'; p++) {
                *p = 'x';
            }
        }
    }
    return outstr;
}

int paramRequired(const ConnInfo *ci, int reqs) {
    int required = 0;
    const char *pw = SAFE_NAME(ci->pwd);

    /* Password is not necessarily a required parameter. */
    if ((reqs & PASSWORD_IS_REQUIRED) != 0)
        if ('\0' == pw[0])
            required |= PASSWORD_IS_REQUIRED;

    return required;
}

#ifdef WIN32
RETCODE
dconn_DoDialog(HWND hwnd, ConnInfo *ci) {
    INT_PTR dialog_result;

    MYLOG(LOG_TRACE, "entering ci = %p", ci);

    if (hwnd) {
        dialog_result =
            DialogBoxParam(s_hModule, MAKEINTRESOURCE(DLG_CONFIG), hwnd,
                           dconn_FDriverConnectProc, (LPARAM)ci);
        if (-1 == dialog_result) {
            int errc = GetLastError();
            MYLOG(LOG_DEBUG, " LastError=%d", errc);
        }
        if (!dialog_result || (dialog_result == -1))
            return SQL_NO_DATA_FOUND;
        else
            return SQL_SUCCESS;
    }

    MYLOG(LOG_DEBUG, " No window specified");
    return SQL_ERROR;
}

INT_PTR CALLBACK dconn_FDriverConnectProc(HWND hdlg, UINT wMsg, WPARAM wParam,
                                          LPARAM lParam) {
    MYLOG(LOG_DEBUG, "dconn_FDriverConnectProc");
    ConnInfo *ci;

    switch (wMsg) {
        case WM_INITDIALOG:
            ci = (ConnInfo *)lParam;

            /* Change the caption for the setup dialog */
            SetWindowText(hdlg, "Database Connection");

            /* Hide the DSN and description fields */
            ShowWindow(GetDlgItem(hdlg, IDC_DSNAMETEXT), SW_HIDE);
            ShowWindow(GetDlgItem(hdlg, IDC_DSNAME), SW_HIDE);

            SetWindowLongPtr(hdlg, DWLP_USER,
                             lParam); /* Save the ConnInfo for the "OK" */
            SetDlgStuff(hdlg, ci);

            SendDlgItemMessage(hdlg, IDC_AUTHTYPE, CB_SETCURSEL, 2, (WPARAM)0);

            if (strcmp(ci->authtype, AUTHTYPE_DEFAULT) == 0) {
                SetFocus(GetDlgItem(hdlg, IDC_USER));
            }
            
            break;

        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
                case IDOK:
                    ci = (ConnInfo *)GetWindowLongPtr(hdlg, DWLP_USER);
                    GetDlgStuff(hdlg, ci);
                case IDCANCEL:
                    EndDialog(hdlg, GET_WM_COMMAND_ID(wParam, lParam) == IDOK);
                    return TRUE;

                case IDOK2:  // <== TEST button
                {
                    ConnInfo tmp_info;
                    ci = (ConnInfo *)GetWindowLongPtr(hdlg, DWLP_USER);
                    GetDlgStuff(hdlg, ci);
                    CC_copy_conninfo(&tmp_info, ci);
                    test_connection(hdlg, &tmp_info, FALSE);
                    CC_conninfo_release(&tmp_info);
                    break;
                }
                case ID_ADVANCED_OPTIONS: {
                    ci = (ConnInfo *)GetWindowLongPtr(hdlg, DWLP_USER);
                    DialogBoxParam(s_hModule,
                                   MAKEINTRESOURCE(DLG_ADVANCED_OPTIONS), hdlg,
                                   advancedOptionsProc, (LPARAM)ci);
                    break;
                }
                case ID_LOG_OPTIONS: {
                    ci = (ConnInfo *)GetWindowLongPtr(hdlg, DWLP_USER);
                    DialogBoxParam(s_hModule, MAKEINTRESOURCE(DLG_LOG_OPTIONS),
                                   hdlg, logOptionsProc, (LPARAM)ci);
                    break;
                }
                case IDC_AUTHTYPE: {
                    ci = (ConnInfo *)GetWindowLongPtr(hdlg, DWLP_USER);
                    const struct authmode *am = GetCurrentAuthMode(hdlg);
                    SetAuthenticationVisibility(hdlg, am);
                    break;
                }
            }
            break;
        case WM_CTLCOLORSTATIC:
            if (lParam == (LPARAM)GetDlgItem(hdlg, IDC_NOTICE_USER)) {
                HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
                SetTextColor((HDC)wParam, RGB(255, 0, 0));
                return (INT_PTR)hBrush;
            }
            break;
    }

    return FALSE;
}
#endif /* WIN32 */

#define ATTRIBUTE_DELIMITER ';'
#define OPENING_BRACKET '{'
#define CLOSING_BRACKET '}'

typedef BOOL (*copyfunc)(ConnInfo *, const char *attribute, const char *value);
BOOL dconn_get_attributes(copyfunc func, const char *connect_string,
                          ConnInfo *ci) {
    BOOL ret = TRUE;
    char *our_connect_string;
    const char *pair, *attribute, *value, *termp;
    BOOL eoftok;
    char *equals, *delp;
    char *strtok_arg;
#ifdef HAVE_STRTOK_R
    char *last = NULL;
#endif /* HAVE_STRTOK_R */

    if (our_connect_string = strdup(connect_string), NULL == our_connect_string) {
        ret = FALSE;
        goto cleanup;
    }
    strtok_arg = our_connect_string;

    if (get_mylog()) {
        char *hide_str = hide_password(our_connect_string);
        MYLOG(LOG_DEBUG, "our_connect_string = '%s'", hide_str);
        free(hide_str);
    }

    termp = strchr(our_connect_string, '\0');
    eoftok = FALSE;
    while (!eoftok) {
        if (strtok_arg != NULL && strtok_arg >= termp) /* for safety */
            break;
#ifdef HAVE_STRTOK_R
        pair = strtok_r(strtok_arg, ";", &last);
#else
        pair = strtok(strtok_arg, ";");
#endif /* HAVE_STRTOK_R */
        if (strtok_arg)
            strtok_arg = NULL;
        if (!pair)
            break;

        equals = strchr(pair, '=');
        if (!equals)
            continue;

        *equals = '\0';
        attribute = pair;   /* ex. DSN */
        value = equals + 1; /* ex. 'CEO co1' */
        /*
         * Values enclosed with braces({}) can contain ; etc
         * We don't remove the braces here because
         * remove_braces() in dlg_specifi.c
         * would remove them later.
         * Just correct the misdetected delimter(;).
         */
        switch (*value) {
            const char *valuen, *closep;

            case OPENING_BRACKET:
                delp = strchr(value, '\0');
                if (delp >= termp) {
                    eoftok = TRUE;
                    break;
                }
                /* Where's a corresponding closing bracket? */
                closep = strchr(value, CLOSING_BRACKET);
                if (NULL != closep && closep[1] == '\0')
                    break;

                for (valuen = value; valuen < termp;
                     closep = strchr(valuen, CLOSING_BRACKET)) {
                    if (NULL == closep) {
                        if (!delp) /* error */
                        {
                            MYLOG(LOG_DEBUG,
                                  "closing bracket doesn't exist 1");
                            ret = FALSE;
                            goto cleanup;
                        }
                        closep = strchr(delp + 1, CLOSING_BRACKET);
                        if (!closep) /* error */
                        {
                            MYLOG(LOG_DEBUG,
                                  "closing bracket doesn't exist 2");
                            ret = FALSE;
                            goto cleanup;
                        }
                        *delp = ATTRIBUTE_DELIMITER; /* restore delimiter */
                        delp = NULL;
                    }
                    if (CLOSING_BRACKET == closep[1]) {
                        valuen = closep + 2;
                        if (valuen >= termp)
                            break;
                        else if (valuen == delp) {
                            *delp = ATTRIBUTE_DELIMITER;
                            delp = NULL;
                        }
                        continue;
                    } else if (ATTRIBUTE_DELIMITER == closep[1]
                               || '\0' == closep[1] || delp == closep + 1) {
                        delp = (char *)(closep + 1);
                        *delp = '\0';
                        strtok_arg = delp + 1;
                        if (strtok_arg + 1 >= termp)
                            eoftok = TRUE;
                        break;
                    }
                    MYLOG(LOG_DEBUG,
                          "subsequent char to the closing bracket is %c "
                          "value=%s",
                          closep[1], value);
                    ret = FALSE;
                    goto cleanup;
                }
        }

        /* Copy the appropriate value to the conninfo  */
        (*func)(ci, attribute, value);
    }

cleanup:
    free(our_connect_string);

    return ret;
}

BOOL dconn_get_DSN_or_Driver(const char *connect_string, ConnInfo *ci) {
    CC_conninfo_init(ci, INIT_GLOBALS);
    return dconn_get_attributes(get_DSN_or_Driver, connect_string, ci);
}

BOOL dconn_get_connect_attributes(const char *connect_string, ConnInfo *ci) {
    return dconn_get_attributes(copyConnAttributes, connect_string, ci);
}
