/*
 *    Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License").
 *    You may not use this file except in compliance with the License.
 *    A copy of the License is located at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    or in the "license" file accompanying this file. This file is distributed
 *    on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 *    express or implied. See the License for the specific language governing
 *    permissions and limitations under the License.
 *
 */

package com.amazon.opendistroforelasticsearch.sql.sql.antlr;

import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertThrows;

import com.amazon.opendistroforelasticsearch.sql.common.antlr.SyntaxCheckException;
import org.junit.jupiter.api.Test;

class SQLSyntaxParserTest {

  private final SQLSyntaxParser parser = new SQLSyntaxParser();

  @Test
  public void canParseQueryEndWithSemiColon() {
    assertNotNull(parser.parse("SELECT 123;"));
  }

  @Test
  public void canParseSelectLiterals() {
    assertNotNull(parser.parse("SELECT 123, 'hello'"));
  }

  @Test
  public void canParseIndexNameWithDate() {
    assertNotNull(parser.parse("SELECT * FROM logs_2020_01"));
    assertNotNull(parser.parse("SELECT * FROM logs-2020-01"));
  }

  @Test
  public void canParseHiddenIndexName() {
    assertNotNull(parser.parse("SELECT * FROM .kibana"));
  }

  @Test
  public void canNotParseIndexNameWithSpecialChar() {
    assertThrows(SyntaxCheckException.class,
        () -> parser.parse("SELECT * FROM hello+world"));
  }

  @Test
  public void canParseIndexNameWithSpecialCharQuoted() {
    assertNotNull(parser.parse("SELECT * FROM `hello+world`"));
    assertNotNull(parser.parse("SELECT * FROM \"hello$world\""));
  }

  @Test
  public void canNotParseIndexNameStartingWithNumber() {
    assertThrows(SyntaxCheckException.class,
        () -> parser.parse("SELECT * FROM 123test"));
  }

  @Test
  public void canNotParseIndexNameSingleQuoted() {
    assertThrows(SyntaxCheckException.class,
        () -> parser.parse("SELECT * FROM 'test'"));
  }

  @Test
  public void canNotParseInvalidSelect() {
    assertThrows(SyntaxCheckException.class,
        () -> parser.parse("SELECT * FROM test WHERE age = 10"));
  }

}