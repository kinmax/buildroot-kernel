#Compile the syscall_test.c
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/syscall_test $BUILDROOT_DIR/custom-scripts/syscall_test.c

#Compile the sleeping_procs.c
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/sleeping_procs $BUILDROOT_DIR/custom-scripts/sleeping_procs.c

make -C $BASE_DIR/../modules/simple_driver/

#Compile the hello write to disk
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/usr/bin/hello_world $BUILDROOT_DIR/disk-test/hello_world.c

$COMPILER -o $BUILDROOT_DIR/output/target/usr/bin/thread_runner $BUILDROOT_DIR/custom-scripts/thread_runner.c -lpthread -lrt

cp $BASE_DIR/../custom-scripts/fstab $BUILDROOT_DIR/output/target/etc/fstab

cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config
