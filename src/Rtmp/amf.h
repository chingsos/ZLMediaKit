﻿/*
 * MIT License
 *
 * Copyright (c) 2016-2019 xiongziliang <771730766@qq.com>
 *
 * This file is part of ZLMediaKit(https://github.com/xiongziliang/ZLMediaKit).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __amf_h
#define __amf_h

#include <assert.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <stdexcept>
#include <functional>
using namespace std;

enum AMFType {
    AMF_NUMBER,
    AMF_INTEGER,
    AMF_BOOLEAN,
    AMF_STRING,
    AMF_OBJECT,
    AMF_NULL,
    AMF_UNDEFINED,
    AMF_ECMA_ARRAY,
    AMF_STRICT_ARRAY,
};

class AMFValue;

class AMFValue {
public:
    friend class AMFEncoder;
    typedef std::map<std::string, AMFValue> mapType;
    typedef std::vector<AMFValue> arrayType;

    AMFValue(AMFType type = AMF_NULL);
    AMFValue(const char *s);
    AMFValue(const std::string &s);
    AMFValue(double n);
    AMFValue(int i);
    AMFValue(bool b);
    AMFValue(const AMFValue &from);
    AMFValue(AMFValue &&from);
    AMFValue &operator =(const AMFValue &from);
    AMFValue &operator =(AMFValue &&from);
    ~AMFValue();

    void clear();
    AMFType type() const ;
    const std::string &as_string() const;
    double as_number() const;
    int as_integer() const;
    bool as_boolean() const;
    string to_string() const;
    const AMFValue &operator[](const char *str) const;
    void object_for_each(const function<void(const string &key, const AMFValue &val)> &fun) const ;
    operator bool() const;
    void set(const std::string &s, const AMFValue &val);
    void add(const AMFValue &val);
private:
    const mapType &getMap() const;
    const arrayType &getArr() const;
    void destroy();
    void init();
private:
    AMFType _type;
    union {
        std::string *string;
        double number;
        int integer;
        bool boolean;
        mapType *object;
        arrayType *array;
    } _value;
};

class AMFDecoder {
public:
    AMFDecoder(const std::string &buf, size_t pos, int version = 0);
    template<typename TP>
    TP load();
private:
    std::string load_key();
    AMFValue load_object();
    AMFValue load_ecma();
    AMFValue load_arr();
    uint8_t front();
    uint8_t pop_front();
private:
    const std::string &buf;
    size_t pos;
    int version;
};

class AMFEncoder {
public:
    AMFEncoder & operator <<(const char *s);
    AMFEncoder & operator <<(const std::string &s);
    AMFEncoder & operator <<(std::nullptr_t);
    AMFEncoder & operator <<(const int n);
    AMFEncoder & operator <<(const double n);
    AMFEncoder & operator <<(const bool b);
    AMFEncoder & operator <<(const AMFValue &value);
    const std::string& data() const ;
    void clear() ;
private:
    void write_key(const std::string &s);
    AMFEncoder &write_undefined();
private:
    std::string buf;
};


#endif
