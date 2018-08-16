# -*- coding: utf-8 -*-
import xlrd
import xlwt
import sys
from datetime import date,datetime


def read_excel():
    workbook = xlrd.open_workbook(r'G:\1-ZigaaCode\script\mytest.xlsx')
    sheet_name = workbook.sheet_names()[0]
    print(sys._getframe().f_lineno,":",sheet_name)
    # sheet = workbook.sheet_by_index(0)
    sheet = workbook.sheet_by_name(sheet_name)
    print(sys._getframe().f_lineno,":",sheet.name,sheet.nrows,sheet.ncols)
    rows = sheet.row_values(13)
    cols = sheet.col_values(2)
    print(sys._getframe().f_lineno,":",rows)
    print(sys._getframe().f_lineno,":",cols)

    print(sys._getframe().f_lineno,":",sheet.cell(19,2).value)
    print(sys._getframe().f_lineno,":",sheet.cell_value(1,0))
    print(sys._getframe().f_lineno,":",sheet.row(1)[0].value)

    print(sys._getframe().f_lineno,":",sheet.cell(19,5).ctype)


if __name__ == '__main__':
    read_excel()
