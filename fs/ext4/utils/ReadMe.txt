All of codes are from android/system/extras/ext4_utils directory.

1.
cd utils
make
mkdir aaaa
./make_ext4fs -l 3840k -b 1024 -g 3840 -J test.img aaaa  ---> ./make_ext4fs -l 3840k -b 1024 -J test.img aaaa
./img2data test.img
./data2img test.img


2.
mkdir bbb
sudo mount -t ext4 -o loop test.img bbbb
ls -l bbbb
sudo umount bbbb

3.
od -tx1 -Ax img-fixnv-org.img


