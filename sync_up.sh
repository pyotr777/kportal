OPT="-anv"
if [[ $1 == "do" ]]; then
    OPT="-av"
else
    echo "Dry run with options \"$OPT\". To actually transfer files execute with \"do\" option: > $0 do"
fi
rsync $OPT --exclude-from 'rsyncexclude.txt' ./ kpinstall:kportal/
