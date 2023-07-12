#! /usr/bin/bash


while getopts c:t: flag
do
  case "${flag}" in
    c) connect=${OPTARG};;
    t) choice=${OPTARG};;
  esac
done

connect="${connect:0:1}"
choice="${choice:0:1}"


if [[ $choice = w && $connect = c ]]; then
  sudo mount -t cifs -o domain=ID,username=c22056601 //websites.cs.cf.ac.uk/c22056601 /mnt/home.cs-2256601;

elif [[ $choice = h && $connect = c ]]; then
  sudo mount -t cifs -o domain=ID,username=c22056601 //home.cs.cf.ac.uk/c22056601 /mnt/home.cs-2256601;

elif [ $connect = d ]; then
  sudo umount /mnt/home.cs-2256601;

else
  echo "not specified wether home or website";

fi

