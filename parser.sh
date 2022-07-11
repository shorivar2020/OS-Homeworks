#!/bin/bash
F=()
while getopts "zh" opt; do
        case "$opt" in
        h) echo "The program defines a link, dir, file. Can make zip from files. Can display help."
            exit 0 ;;
        z) FLAG=1;;
        *) exit 2 ;;
        esac
done
while read -r P; do
    if [ "${P::5}" == "PATH " ]; then
        if [ -L "${P:5}" ]; then
            CILL="$(readlink "${P:5}")"
            echo "LINK '${P:5}' '${CILL}'"
        elif [ -f "${P:5}" ]; then
            COUNT_LINE="$(< "${P:5}" wc -l)"
            FIRST_LINE="$(head -n 1 "${P:5}")"
            if [ ! -r "${P:5}" ]; then
                exit 2
            fi
            echo "FILE '${P:5}' ${COUNT_LINE} '${FIRST_LINE}'"
            F+=( "${P:5}" )
        elif [ -d "${P:5}" ]; then
            echo "DIR '${P:5}'"
        elif [ ! -f "${P:5}" ]; then
            >&2 echo "ERROR '${P:5}'"
            exit 1
        fi
    fi
    if [ "$FLAG" == 1 ]; then
        tar czf output.tgz "${F[@]}"
    fi
done
exit 0