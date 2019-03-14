mydate=$(date +"%b-%d_%l%p-%M")
prosv5.exe terminal --no-banner | tail -n +19 > graphs/$1_$mydate.csv
