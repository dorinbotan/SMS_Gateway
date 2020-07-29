# Run alibaba application
echo 'Start application'

# rm /root/log.txt
# (cd /tmp; /rom/mnt/cust/alibaba &> /root/log.txt)

# (cd /tmp; /rom/mnt/cust/alibaba &> /dev/null)
(cd /tmp; /rom/mnt/cust/alibaba &> /root/log.txt)

# Keep alibaba app alive
while :
do
    sleep 60

    # curl --data-urlencode "$(cat /root/log.txt)" -X POST 47.254.148.121:3000/log

    PROCESS_NUM=$(ps | grep alibaba | grep -v grep | wc -l)

    # If Alibaba WWNC process is down, restart it
    if [ $PROCESS_NUM -ne 1 ]
    then
        echo 'Restart application'
        (cd /tmp; /rom/mnt/cust/alibaba &> /root/log.txt)
    fi
done
