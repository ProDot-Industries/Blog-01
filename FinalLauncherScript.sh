echo "script starting"
mkdir project
mkdir project/captured
count=1
while [ $count -lt 10000 ]
do
    echo "count = " . $count
    python cameraVideoMySQL.py
    count=`expr $count + 1`
    echo "restarting python script"
done
echo "rebooting system in 30 sec"
sec=30
while [ $sec le 0 ]
do
    sec=`expr $sec - 1`
    echo "rebooting in " . $sec
    sleep 1
done
echo "exit"
echo "PiPassword" | sudo -S reboot now
