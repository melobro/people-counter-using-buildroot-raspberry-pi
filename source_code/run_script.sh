#!/bin/sh

### BEGIN INIT INFO
# Provides:          pi_code
# Required-Start:    $local_fs $remote_fs $network $syslog
# Required-Stop:     $local_fs $remote_fs $network $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start pi_code at boot time
# Description:       Enable service provided by pi_code.
### END INIT INFO

case "$1" in
  start)
    echo "Starting pi_code"
    # Command to start the process
    /home/vboxuser/Rasp/better &
    ;;
  stop)
    echo "Stopping pi_code"
    # Command to stop the process, if needed
    ;;
  restart)
    echo "Restarting pi_code"
    $0 stop
    sleep 1
    $0 start
    ;;
  status)
    # Check if the process is running
    if pgrep -x "pi_code" > /dev/null
    then
      echo "pi_code is running"
    else
      echo "pi_code is not running"
    fi
    ;;
  *)
    echo "Usage: $0 {start|stop|restart|status}"
    exit 1
    ;;
esac

exit 0
