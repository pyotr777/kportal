echo "Find out all daemon from the login nodes"
i=2
while [ $i -lt 10 ];
do
  echo "At kogin$i node: "
  echo "  find out all kdeskdeamon progress"
  pids=$( ssh klogin$i 'ps aufx | sort -nr -k 4 | grep "[k]deskdaemon"' | awk '{ print $2 }' )
  if [ "$pids" = "" ]
  then
    echo "    not found"
  else
    echo "    pids: $pids found."
    echo "  kill them now"
    for pid in $pids; do
      ssh klogin$i "kill $pid"
      echo "    kill $pid"
    done
  fi
  let i=i+1
done
echo "Done"
