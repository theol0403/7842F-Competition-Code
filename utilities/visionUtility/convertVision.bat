echo off

fsutil file createnew input.txt 0 >NUL
echo paste output into input.txt
notepad input.txt
echo you pasted:
cat input.txt
echo.

echo.
echo output is:
call JREPL.BAT "\s*(?:vex::)?vision::signature\s+([a-zA-Z\d_]+)\s*\((\d)([\d\,\s\-\.]*)\);" "pros::vision_signature_s_t $1 = pros::Vision::signature_from_utility($2$3); sensor->set_signature($2, &$1);" /F input.txt /A | grep -v ", 0, 0, 0, 0, 0, 0, 3.000, 0)" | tee output.txt
echo.

set outputFormat=c
set /p outputFormat=output to notepad or clipboard [n/c]?:

IF "%outputFormat%"=="c" (
cat output.txt | clip
echo copied to clipboard!
) ELSE (
notepad output.txt
)

rm input.txt
rm output.txt
