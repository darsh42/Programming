#!/bin/bash

while getopts c:config: flag
do
  case "${flag}" in
    c) connect=${OPTARG};;
    config) VPN=${OPTARG};;
  esac
done

connect="${connect:0:1}"


if [[ $connect = c ]]; then
  openvpn3 session-start --config $VPN;
elif [[ $connect = d ]]; then
  openvpn3 session-manage --disconnect --config $VPN;
else
  echo "not specified connect or diconnect";
fi

