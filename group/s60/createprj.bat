devices -setdefault @s60_2nd
call bldmake bldfiles
call abld reallyclean
call makmake FlashSms.mmp vc6
pause