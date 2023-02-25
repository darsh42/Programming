while getopts f: flag
do
  case "${flag}" in
    f) filename = ${OPTARG};;
  esac
done

sudo as "$filename.s" -o "$filename.o"
sudo ld "$filename.o" -o "$filename"
sudo exec "$filename"
sudo echo $?
