/**
 * The MIT License 
 * 
 * Copyright (c) 2019 Illusion Lee
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

#include <string.h>
#include "solar_terms.h"

/* 节气偏移表 solar_terms_day_offset_table[0] 数据对应的起始年份 */
#define SOLAR_TERMS_DAY_OFFSET_TABLE_START_YEAR 1900

/* 公历1-12月对应二十四节气名称 */
const char *solar_terms_const[] = {
    "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨",
    "立夏", "小满", "芒种", "夏至", "小暑", "大暑", "立秋", "处暑",
    "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至"};
/* 公历1-12月对应二十四节气基本节气日,[0][12]为1900-2059年基本节气日，[1][12]为2060-2100年基本节气日 */
const unsigned int solar_terms_day_table[2][12] =
    {
        {0x061404, 0x041306, 0x061505, 0x051406, 0x061506, 0x061507, 0x071708, 0x081708, 0x081708, 0x081808, 0x081607, 0x071606},
        {0x051404, 0x041305, 0x051405, 0x051405, 0x051505, 0x051507, 0x071707, 0x071707, 0x071708, 0x081707, 0x071607, 0x071605}};
/* 实际节气日与基本节气日偏移量，范围[1900, 2100] ，但get_solar_terms()函数输入的年份范围为[1900, 2099]，bit = 1 表示存在+-1天偏移，bit = 0 表示无偏移 */
unsigned int solar_terms_day_offset_table[] =
    {
        /* 1900-1909 */
        0x001024, 0x415966, 0x637D67, 0x7FFFE7, 0xF01024, 0x415966, 0x635D67, 0x7FFFE7, 0xF01024, 0x415966,
        /* 1910-1919 */
        0x635D67, 0x7F7FE7, 0xF01000, 0x015926, 0x415D67, 0x777DE7, 0x701000, 0x011926, 0x415D66, 0x777DE7,
        /* 1920-1929 */
        0x701000, 0x001924, 0x415D66, 0x737DE7, 0x701000, 0x001924, 0x415966, 0x637D67, 0x700018, 0x001024,
        /* 1930-1939 */
        0x415966, 0x637D67, 0x700018, 0x001024, 0x415966, 0x635D67, 0x700018, 0x001024, 0x415966, 0x635D67,
        /* 1940-1949 */
        0x700018, 0x001024, 0x415966, 0x635D67, 0x708018, 0x001000, 0x015926, 0x415D67, 0x788218, 0x801000,
        /* 1950-1959 */
        0x001926, 0x415D67, 0x788218, 0x801000, 0x001924, 0x415966, 0x7C8218, 0x801000, 0x001024, 0x415966,
        /* 1960-1969 */
        0x6C8298, 0x800018, 0x001024, 0x415966, 0x6C8298, 0x800018, 0x001024, 0x415966, 0x6CA298, 0x800018,
        /* 1970-1979 */
        0x001024, 0x415966, 0x6CA298, 0x808018, 0x001024, 0x415926, 0x6EA298, 0x808218, 0x001004, 0x411926,
        /* 1980-1989 */
        0x6EA298, 0x808218, 0x001000, 0x001926, 0x4EA698, 0x888218, 0x801000, 0x001124, 0x4EA699, 0x8C8298,
        /* 1990-1999 */
        0x800000, 0x001024, 0x4EA699, 0x9C8298, 0x800018, 0x001024, 0x4EA699, 0x9CA298, 0x800018, 0x001024,
        /* 2000-2009 */
        0x4EA699, 0x9CA298, 0x800018, 0x001024, 0x4EA699, 0x9CA298, 0x808218, 0x001024, 0x4EA6D9, 0x9EA298,
        /* 2010-2019 */
        0x808218, 0x001004, 0x4EE6D9, 0x9EA698, 0x808218, 0x001000, 0x0FEED9, 0xBEA698, 0x888218, 0x800000,
        /* 2020-2029 */
        0x0FEFDB, 0xBEA699, 0x8C8298, 0x800000, 0x0FEFDB, 0xBEA699, 0x9CA298, 0x800018, 0x0FEFDB, 0xBEA699,
        /* 2030-2039 */
        0x9CA298, 0x800018, 0x0FEFDB, 0xBEA699, 0x9CA298, 0x808218, 0x0FEFDB, 0xBEA6D9, 0x9CA298, 0x808218,
        /* 2040-2049 */
        0x0FEFFB, 0xBEE6D9, 0x9EA698, 0x808218, 0x0FEFFB, 0xBFE6D9, 0x9EA698, 0x808218, 0x0FFFFF, 0xFFEFD9,
        /* 2050-2059 */
        0xBEA698, 0x888298, 0x8FFFFF, 0xFFEFD9, 0xBEA698, 0x8CA298, 0x8FFFFF, 0xFFEFDB, 0xBEA699, 0x9CA298,
        /* 2060-2069 */
        0x035D7F, 0x734D43, 0x320401, 0x100000, 0x035D7F, 0x734D43, 0x320401, 0x100000, 0x03DF7F, 0x734D43,
        /* 2070-2079 */
        0x324441, 0x100000, 0x03DF7F, 0x734D63, 0x324441, 0x120400, 0x03DF7F, 0x734D63, 0x334C41, 0x120400,
        /* 2080-2089 */
        0x03DF7F, 0x735D67, 0x334D41, 0x120400, 0x0BDFFF, 0x735D67, 0x734D41, 0x320400, 0x0FFFFF, 0xF35D67,
        /* 2090-2099 */
        0x734D43, 0x320401, 0x0FFFFF, 0xF35D7F, 0x734D43, 0x320401, 0x1FFFFF, 0xF3DF7F, 0x734D43, 0x320401,
        /* 2100 */
        0x100000};

/* 公历月份可能最大的天数 */
const unsigned char month_days_max_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * 公历年月转换为当月两个节气日期以及下个月节气日期。
 *
 * @param year 公历年份 [1900-2099]。
 * @param month 公历月份 [1-12]。
 *
 * @return -1 失败；其它：bit[11:8]：当月节气的节日期，bit[7:4]：当月节气的气日期，bit[3:0]：下个月节气的节日期。
 * 注：年份输入最大值必须比solar_terms_day_offset_table[]节气偏差表最后一个元素数据对应的年份至少小1
 */
int get_solar_terms(int year, int month)
{
    unsigned char solar_terms_day_table_y, solar_terms_day_offset_table_char, solar_terms_day_offset_table_char_bit;
    unsigned int solar_terms_days_find;

    if ((year < SOLAR_TERMS_DAY_OFFSET_TABLE_START_YEAR) || (year > (SOLAR_TERMS_DAY_OFFSET_TABLE_START_YEAR + sizeof(solar_terms_day_offset_table) / sizeof(int)) - 2) || month < 1 || month > 12)
        return -1;
    /* 下个月节气的节位置 */
    solar_terms_day_table_y = ((month - 1 + 1) % 12) << 1;
    /* 下个月节气的偏移量所在的年节气日偏移量表的字节 */
    if (month < 12)
        solar_terms_day_offset_table_char = (unsigned char)(solar_terms_day_offset_table[year - SOLAR_TERMS_DAY_OFFSET_TABLE_START_YEAR] >> (16 - ((solar_terms_day_table_y >> 3) << 3)));
    else
        solar_terms_day_offset_table_char = (unsigned char)(solar_terms_day_offset_table[year + 1 - SOLAR_TERMS_DAY_OFFSET_TABLE_START_YEAR] >> (16 - ((solar_terms_day_table_y >> 3) << 3)));
    /* 下个月节气的节偏移量 */
    solar_terms_day_offset_table_char_bit = ((solar_terms_day_offset_table_char << (solar_terms_day_table_y % 8)) & 0x80);
    /* 当月节气的节位置 */
    solar_terms_day_table_y = (month - 1) << 1;
    solar_terms_day_offset_table_char = (unsigned char)(solar_terms_day_offset_table[year - SOLAR_TERMS_DAY_OFFSET_TABLE_START_YEAR] >> (16 - ((solar_terms_day_table_y >> 3) << 3)));
    /* 当月节气的节和气偏移量，并合并下个月节气的节偏移量 */
    solar_terms_day_offset_table_char_bit = ((solar_terms_day_offset_table_char << (solar_terms_day_table_y % 8)) & 0xC0) | (solar_terms_day_offset_table_char_bit >> 2);
    /* 当月节气的节所对应的基本节气日 */
    solar_terms_days_find = year < 2060 ? solar_terms_day_table[0][solar_terms_day_table_y >> 1] : solar_terms_day_table[1][solar_terms_day_table_y >> 1];
    /* 2059年12月对应的节气基本节气日-1为2060年1月对应的节气基本节气日中的节 */
    if (year == 2059 && month == 12)
    {
        solar_terms_days_find--;
    }
    /* 依次判断当月节气中的节、气、下个月节气中的节是否要与基本节气日进行+-1偏差校正 */
    for (int i = 0; i < 3; i++)
    {
        if (((solar_terms_day_offset_table_char_bit << i) & 0x80) != 0)
        {
            if (year > 2015)
            {
                solar_terms_days_find -= (1 << ((2 - i) << 3));
            }
            else if (year < 1944)
            {
                if (solar_terms_day_table_y == 19 || solar_terms_day_table_y == 20)
                    solar_terms_days_find -= (1 << ((2 - i) << 3));
                else
                    solar_terms_days_find += (1 << ((2 - i) << 3));
            }
            else
            {
                if (solar_terms_day_table_y == 1 || solar_terms_day_table_y == 2 || (year < 1993 && solar_terms_day_table_y == 3)
                    || solar_terms_day_table_y == 7 || (year < 2012 && solar_terms_day_table_y == 9) || solar_terms_day_table_y == 11
                    || solar_terms_day_table_y == 12 || (year < 1984 && solar_terms_day_table_y == 13) || solar_terms_day_table_y == 15
                    || (year < 2008 && solar_terms_day_table_y == 17) || solar_terms_day_table_y == 18 || solar_terms_day_table_y == 21
                    || solar_terms_day_table_y == 22 || (year < 1988 && solar_terms_day_table_y == 23))
                    solar_terms_days_find += (1 << ((2 - i) << 3));
                else
                    solar_terms_days_find -= (1 << ((2 - i) << 3));
            }
        }
        /* 下一个节气 */
        solar_terms_day_table_y = (solar_terms_day_table_y + 1) % 24;
        /* 如果当前月为12月，下个月节气中的节的年份+1 */
        if (month == 12 && i == 1)
            year++;
    }
    /* 当月节气的节日期，bit[7:4]：当月节气的气日期，bit[3:0]：下个月节气的节日期 */
    return solar_terms_days_find;
}

/**
 * 公历年月日判断与最近的下一个节气日的剩余天数。
 *
 * @param year 公历年份 [1900-2099]。
 * @param month 公历月份 [1-12]。
 * @param day 公历日 [1-31]。
 * @param solar_terms_str 返回节气日字符串。
 *
 * @return -1 失败；0 当天为节气日；其它：距离最近的下一个节气日的剩余天数。
 */
signed char sun2solar_terms(int year, int month, int day, char *solar_terms_str)
{
    unsigned char solar_terms_day_table_y, month_days_max;
    unsigned int solar_terms_days_find;

    if ((year < SOLAR_TERMS_DAY_OFFSET_TABLE_START_YEAR) || (year > (SOLAR_TERMS_DAY_OFFSET_TABLE_START_YEAR + sizeof(solar_terms_day_offset_table) / sizeof(int)) - 2) || month < 1 || month > 12)
        return -1;
    /* 查找当月的最大天数 */
    month_days_max = ((month == 2) && (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0))) ? (month_days_max_table[month - 1] + 1) : month_days_max_table[month - 1];
    /* 判断输入的日是否合法 */
    if (month_days_max - day < 0)
        return -1;
    solar_terms_days_find = get_solar_terms(year, month);
    /* 当天依次与当月节气的节、气、下个月节气的节进行对比 */
    if (day <= (unsigned char)(solar_terms_days_find >> 16))
    {
        solar_terms_day_table_y = (month - 1) << 1;
        strcat(solar_terms_str, solar_terms_const[solar_terms_day_table_y]);
        return ((unsigned char)(solar_terms_days_find >> 16) - day);
    }
    else if (day <= (unsigned char)(solar_terms_days_find >> 8))
    {
        solar_terms_day_table_y = ((month - 1) << 1) + 1;
        strcat(solar_terms_str, solar_terms_const[solar_terms_day_table_y]);
        return ((unsigned char)(solar_terms_days_find >> 8) - day);
    }
    else
    {
        solar_terms_day_table_y = ((month - 1 + 1) % 12) << 1;
        strcat(solar_terms_str, solar_terms_const[solar_terms_day_table_y]);
        return (month_days_max - day + (unsigned char)solar_terms_days_find);
    }
}
