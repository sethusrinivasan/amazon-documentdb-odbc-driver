﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <string>
#include <vector>
#include <utility>
#include <memory>

namespace Aws {
namespace Utils {
namespace Json {
class JsonValue;
class JsonView;
}  // namespace Json
}  // namespace Utils
}  // namespace Aws

class Row;

  /**
   * <p> Datum represents a single data point in a query result. </p><p><h3>See
   */
  class Datum
  {
  public:
    Datum();
    Datum(Aws::Utils::Json::JsonView jsonValue);
    Datum& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p> Indicates if the data point is a scalar value such as integer, string,
     * double, or boolean. </p>
     */
    inline const std::string& GetScalarValue() const{ return m_scalarValue; }

    /**
     * <p> Indicates if the data point is a scalar value such as integer, string,
     * double, or boolean. </p>
     */
    inline bool ScalarValueHasBeenSet() const { return m_scalarValueHasBeenSet; }

    /**
     * <p> Indicates if the data point is a scalar value such as integer, string,
     * double, or boolean. </p>
     */
    inline void SetScalarValue(const std::string& value) { m_scalarValueHasBeenSet = true; m_scalarValue = value; }

    /**
     * <p> Indicates if the data point is a scalar value such as integer, string,
     * double, or boolean. </p>
     */
    inline void SetScalarValue(std::string&& value) { m_scalarValueHasBeenSet = true; m_scalarValue = std::move(value); }

    /**
     * <p> Indicates if the data point is a scalar value such as integer, string,
     * double, or boolean. </p>
     */
    inline void SetScalarValue(const char* value) { m_scalarValueHasBeenSet = true; m_scalarValue.assign(value); }

    /**
     * <p> Indicates if the data point is a scalar value such as integer, string,
     * double, or boolean. </p>
     */
    inline Datum& WithScalarValue(const std::string& value) { SetScalarValue(value); return *this;}

    /**
     * <p> Indicates if the data point is a scalar value such as integer, string,
     * double, or boolean. </p>
     */
    inline Datum& WithScalarValue(std::string&& value) { SetScalarValue(std::move(value)); return *this;}

    /**
     * <p> Indicates if the data point is a scalar value such as integer, string,
     * double, or boolean. </p>
     */
    inline Datum& WithScalarValue(const char* value) { SetScalarValue(value); return *this;}



    /**
     * <p> Indicates if the data point is an array. </p>
     */
    inline const std::vector<Datum>& GetArrayValue() const{ return m_arrayValue; }

    /**
     * <p> Indicates if the data point is an array. </p>
     */
    inline bool ArrayValueHasBeenSet() const { return m_arrayValueHasBeenSet; }

    /**
     * <p> Indicates if the data point is an array. </p>
     */
    inline void SetArrayValue(const std::vector<Datum>& value) { m_arrayValueHasBeenSet = true; m_arrayValue = value; }

    /**
     * <p> Indicates if the data point is an array. </p>
     */
    inline void SetArrayValue(std::vector<Datum>&& value) { m_arrayValueHasBeenSet = true; m_arrayValue = std::move(value); }

    /**
     * <p> Indicates if the data point is an array. </p>
     */
    inline Datum& WithArrayValue(const std::vector<Datum>& value) { SetArrayValue(value); return *this;}

    /**
     * <p> Indicates if the data point is an array. </p>
     */
    inline Datum& WithArrayValue(std::vector<Datum>&& value) { SetArrayValue(std::move(value)); return *this;}

    /**
     * <p> Indicates if the data point is an array. </p>
     */
    inline Datum& AddArrayValue(const Datum& value) { m_arrayValueHasBeenSet = true; m_arrayValue.push_back(value); return *this; }

    /**
     * <p> Indicates if the data point is an array. </p>
     */
    inline Datum& AddArrayValue(Datum&& value) { m_arrayValueHasBeenSet = true; m_arrayValue.push_back(std::move(value)); return *this; }


    /**
     * <p> Indicates if the data point is a row. </p>
     */
    const Row& GetRowValue() const;

    /**
     * <p> Indicates if the data point is a row. </p>
     */
    bool RowValueHasBeenSet() const;

    /**
     * <p> Indicates if the data point is a row. </p>
     */
    void SetRowValue(const Row& value);

    /**
     * <p> Indicates if the data point is a row. </p>
     */
    void SetRowValue(Row&& value);

    /**
     * <p> Indicates if the data point is a row. </p>
     */
    Datum& WithRowValue(const Row& value);

    /**
     * <p> Indicates if the data point is a row. </p>
     */
    Datum& WithRowValue(Row&& value);


    /**
     * <p> Indicates if the data point is null. </p>
     */
    inline bool GetNullValue() const{ return m_nullValue; }

    /**
     * <p> Indicates if the data point is null. </p>
     */
    inline bool NullValueHasBeenSet() const { return m_nullValueHasBeenSet; }

    /**
     * <p> Indicates if the data point is null. </p>
     */
    inline void SetNullValue(bool value) { m_nullValueHasBeenSet = true; m_nullValue = value; }

    /**
     * <p> Indicates if the data point is null. </p>
     */
    inline Datum& WithNullValue(bool value) { SetNullValue(value); return *this;}

  private:

    std::string m_scalarValue;
    bool m_scalarValueHasBeenSet;

    std::vector<Datum> m_arrayValue;
    bool m_arrayValueHasBeenSet;

    std::shared_ptr<Row> m_rowValue;
    bool m_rowValueHasBeenSet;

    bool m_nullValue;
    bool m_nullValueHasBeenSet;
  };

