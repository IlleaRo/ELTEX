read -rp "Process name: " prog_Name
read -rp "Survey frequency in seconds: " frequency

$prog_Name &
while true
do
    if [[ -n "$(pgrep "$prog_Name")" ]]
    then
        echo "[INFO] Process is working now!"
    else
        echo "[INFO] Process was stopped. Script will start it..."
        $prog_Name &
    fi
    sleep "$frequency"
done
