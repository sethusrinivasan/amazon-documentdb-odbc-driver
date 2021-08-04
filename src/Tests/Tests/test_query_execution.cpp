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

// clang-format off
#include "helper.h"
#include "odbc_communication.h"
#include "pch.h"
#include "unit_test_helper.h"
// clang-format on

//const std::string valid_port = "9200";
//const std::string valid_user = "admin";
//const std::string valid_pw = "admin";
//const std::string valid_region = "us-west-3";
//const std::string query =
//    "SELECT Origin FROM kibana_sample_data_flights LIMIT 5";
//const std::string all_columns_flights_query =
//    "SELECT * FROM kibana_sample_data_flights LIMIT 5";
//const std::string some_columns_flights_query =
//    "SELECT Origin, OriginWeather FROM kibana_sample_data_flights LIMIT 5";
//const std::string invalid_query = "SELECT";
//const int EXECUTION_SUCCESS = 0;
//const int EXECUTION_ERROR = -1;
//const int all_columns_flights_count = 25;
//const int some_columns_flights_count = 2;
//runtime_options valid_conn_opt_val = {
//    {"IAM", valid_user, valid_pw, "", valid_region}};

// TEST(TestESExecDirect, ValidQuery) {
    //TSCommunication conn;
    //ASSERT_TRUE(conn.ConnectionOptions(valid_conn_opt_val));
    //ASSERT_TRUE(conn.ConnectDBStart());
    //EXPECT_EQ(EXECUTION_SUCCESS,
    //    ESExecDirect(&conn, some_columns_flights_query.c_str()));
// }

// TEST(TestTSExecDirect, MissingQuery) {
    //TSCommunication conn;
    //ASSERT_TRUE(conn.ConnectionOptions(valid_conn_opt_val));
    //ASSERT_TRUE(conn.ConnectDBStart());
    //EXPECT_EQ(EXECUTION_ERROR, ESExecDirect(&conn, NULL));
// }

//TEST(TesTSExecDirect, MissingConnection) {
//    EXPECT_EQ(EXECUTION_ERROR,
//              ESExecDirect(NULL));
//}

// Conn::ExecDirect

//TEST(TestConnExecDirect, ValidQueryAllColumns) {
//    TSCommunication conn;
//    ASSERT_TRUE(conn.ConnectionOptions(valid_conn_opt_val));
//    ASSERT_TRUE(conn.ConnectDBStart());
//
//    conn.ExecDirect(all_columns_flights_query.c_str());
//    ESResult* result = conn.PopResult();
//    EXPECT_EQ("SELECT", result->command_type);
//    EXPECT_FALSE(result->result_json.empty());
//    EXPECT_EQ(all_columns_flights_count, result->num_fields);
//    EXPECT_EQ((size_t)all_columns_flights_count, result->column_info.size());
//}

//TEST(TestConnExecDirect, ValidQuerySomeColumns) {
//    TSCommunication conn;
//    ASSERT_TRUE(conn.ConnectionOptions(valid_conn_opt_val));
//    ASSERT_TRUE(conn.ConnectDBStart());
//
//    conn.ExecDirect(some_columns_flights_query.c_str());
//    ESResult* result = conn.PopResult();
//    EXPECT_EQ("SELECT", result->command_type);
//    EXPECT_FALSE(result->result_json.empty());
//    EXPECT_EQ(some_columns_flights_count, result->num_fields);
//    EXPECT_EQ((size_t)some_columns_flights_count, result->column_info.size());
//}
//
//TEST(TestConnExecDirect, InvalidQuery) {
//    TSCommunication conn;
//    ASSERT_TRUE(conn.ConnectionOptions(valid_conn_opt_val));
//    ASSERT_TRUE(conn.ConnectDBStart());
//
//    conn.ExecDirect(invalid_query.c_str());
//    ESResult* result = conn.PopResult();
//    EXPECT_EQ(NULL, (void*)result);
//}
//
//// Conn::PopResult
//
//TEST(TestConnPopResult, PopEmptyQueue) {
//    TSCommunication conn;
//    ASSERT_TRUE(conn.ConnectionOptions(valid_conn_opt_val));
//    ASSERT_TRUE(conn.ConnectDBStart());
//
//    ESResult* result = conn.PopResult();
//    EXPECT_EQ(NULL, (void*)result);
//}
//
//TEST(TestConnPopResult, PopTwoQueryResults) {
//    TSCommunication conn;
//    ASSERT_TRUE(conn.ConnectionOptions(valid_conn_opt_val));
//    ASSERT_TRUE(conn.ConnectDBStart());
//
//    conn.ExecDirect(some_columns_flights_query.c_str());
//    conn.ExecDirect(all_columns_flights_query.c_str());
//
//    // Pop some_columns
//    ESResult* result = conn.PopResult();
//    EXPECT_EQ(some_columns_flights_count, result->num_fields);
//
//    // Pop all_columns
//    result = conn.PopResult();
//    EXPECT_EQ(all_columns_flights_count, result->num_fields);
//}