dim oExcel, oWb, oSheet 
dim srcStr, resStr, line
Set oExcel= CreateObject("Excel.Application") 
Set oWb = oExcel.Workbooks.Open("G:\1-ZigaaCode\script\mytest.xlsx")
Set oSheet = oWb.Sheets("Sheet1") 
rem MsgBox oSheet.Range("C2").Value   rem 显示特定单元格的内容
rem MsgBox oSheet.Cells(4,2).Value
line=InputBox("Enter the line:")
Call replaceStr(oSheet, "L1", "FAI20", line)
Call replaceStr(oSheet, "L2", "FAI19", line)
Call replaceStr(oSheet, "L3", "FAI18", line)
Call replaceStr(oSheet, "W1", "FAI16", line)
Call replaceStr(oSheet, "W2", "FAI22", line)
Call replaceStr(oSheet, "T1", "FAI10_1", line)
Call replaceStr(oSheet, "T2", "FAI10_2", line)
Call replaceStr(oSheet, "T3", "FAI10_3", line)
Call replaceStr(oSheet, "T4", "FAI10_4", line)
Call replaceStr(oSheet, "T3", "FAI10_3", line)
Call replaceStr(oSheet, "T4", "FAI10_4", line)
rem oSheet.Range("C2").Value = "1_FAI19_avg"
rem oExcel = Replace(oExcel, "1_L1_avg", "1_FAI19_avg")
oExcel.Visible = True rem 显示当前窗口
rem oExcel.Caption = "应用程序调用 Microsoft Excel" rem 更改表格标题栏
rem oSheet.columns(3).Replace  src res
rem Cells.Replace What:="1_L1_avg", Replacement:="1_FAI19_avg", LookAt:=xlPart, SearchOrder:=xlByRows, MatchCase:=False, SearchFormat:=False, ReplaceFormat:=False
oWb.Save
oWb.Close
oExcel.Quit
Set oExcel = nothing
Set vbs=CreateObject("Wscript.Shell")
vbs.Run "taskkill /F /IM Excel.exe"

MsgBox "Done"

Sub replaceStr(sheet,str, str1,line)
  For i=2 to 100
    ExcValue1 = sheet.Cells(line,i).Value
	rem ExcValue2 = sheet.Cells(19,i).Value
    If InStr(ExcValue1, str)>-1 then
      rem exit For
	  ExcValue = Replace(ExcValue1, str, str1)
	  sheet.Cells(line,i).Value = ExcValue
	  rem sheet.Cells(i,19).Value = "1_FAI19_avg"
    End If
  Next
End Sub



rem 用于excel中的VBA宏
'Sub Find_Replace(oSheet)
'    src = InputBox("Enter the src replace string:")
'    res = InputBox("Enter the res replace string:")
'    oSheet.cells.Replace What:=src, Replacement:=res, _
'      LookAt:=xlPart, SearchOrder:=xlByColumns, MatchCase:=True, _
'	  SearchFormat:=True, ReplaceFormat:=True
'End Sub
