/* Copyright (C) 2010 Zsolt Sz Sztupák
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#include "bootloader.h"
#include "common.h"
#include "cutils/properties.h"
#include "install.h"
#include "minui/minui.h"
#include "minzip/DirUtil.h"
#include "roots.h"
#include "recovery_ui.h"

#include "extendedcommands.h"
#include "commands.h"
#include "lagfixutils.h"
#include "nandroid.h"

extern char **environ;

void apply_ln(char* name) {
  char tmp[128];
  sprintf(tmp,"ln -s /system/xbin/busybox /system/xbin/%s",name);
  __system(tmp);
}

void apply_rm(char* name) {
  char tmp[128];
  sprintf(tmp,"/system/xbin/rm /system/bin/%s",name);
  __system(tmp);
}

void apply_root_to_device(int mode) {
  ensure_root_path_mounted("SYSTEM:");
  ui_print("Copying Superuser and Busybox files to /system/xbin\n");

  ui_print("Copying su binary\n");
  __system("rm /system/bin/su");
  __system("rm /system/xbin/su");
  __system("cp /res/misc/su /system/xbin/su");
  __system("chown 0.0 /system/xbin/su");
  __system("chmod 4755 /system/xbin/su");

  ui_print("Copying Superuser.apk\n");
  __system("rm /system/app/Superuser.apk");
  __system("rm /data/app/Superuser.apk");
  __system("cp /res/misc/Superuser.apk /system/app/Superuser.apk");
  __system("chown 0.0 /system/app/Superuser.apk");
  __system("chmod 644 /system/app/Superuser.apk");

  ui_print("Copying Busybox executable\n");
  __system("rm /system/xbin/busybox");
  __system("rm /system/bin/busybox");
  __system("cp /sbin/recovery /system/xbin/busybox");
  __system("chmod 755 /system/xbin/busybox");

  ui_print("Creating symlinks to commands\n");

  apply_ln("[");
  apply_ln("[[");
  apply_ln("ash");
  apply_ln("awk");
  apply_ln("basename");
  apply_ln("bbconfig");
  apply_ln("bunzip2");
  apply_ln("bzcat");
  apply_ln("bzip2");
  apply_ln("cal");
  apply_ln("cat");
  apply_ln("catv");
  apply_ln("chgrp");
  apply_ln("chmod");
  apply_ln("chown");
  apply_ln("chroot");
  apply_ln("cksum");
  apply_ln("clear");
  apply_ln("cmp");
  apply_ln("cp");
  apply_ln("cpio");
  apply_ln("cut");
  apply_ln("date");
  apply_ln("dc");
  apply_ln("dd");
  apply_ln("depmod");
  apply_ln("devmem");
  apply_ln("df");
  apply_ln("diff");
  apply_ln("dirname");
  apply_ln("dmesg");
  apply_ln("dos2unix");
  apply_ln("du");
  apply_ln("echo");
  apply_ln("egrep");
  apply_ln("env");
  apply_ln("expr");
  apply_ln("false");
  apply_ln("fdisk");
  apply_ln("fgrep");
  apply_ln("find");
  apply_ln("fold");
  apply_ln("free");
  apply_ln("freeramdisk");
  apply_ln("fuser");
  apply_ln("getopt");
  apply_ln("grep");
  apply_ln("gunzip");
  apply_ln("gzip");
  apply_ln("head");
  apply_ln("hexdump");
  apply_ln("id");
  apply_ln("insmod");
  apply_ln("install");
  apply_ln("kill");
  apply_ln("killall");
  apply_ln("killall5");
  apply_ln("length");
  apply_ln("less");
  apply_ln("ln");
  apply_ln("losetup");
  apply_ln("ls");
  apply_ln("lsmod");
  apply_ln("lspci");
  apply_ln("lsusb");
  apply_ln("lzop");
  apply_ln("lzopcat");
  apply_ln("md5sum");
  apply_ln("mkdir");
  apply_ln("mke2fs");
  apply_ln("mkfifo");
  apply_ln("mkfs.ext2");
  apply_ln("mknod");
  apply_ln("mkswap");
  apply_ln("mktemp");
  apply_ln("modprobe");
  apply_ln("more");
  apply_ln("mount");
  apply_ln("mountpoint");
  apply_ln("mv");
  apply_ln("nice");
  apply_ln("nohup");
  apply_ln("od");
  apply_ln("patch");
  apply_ln("pgrep");
  apply_ln("pidof");
  apply_ln("pkill");
  apply_ln("printenv");
  apply_ln("printf");
  apply_ln("ps");
  apply_ln("pwd");
  apply_ln("rdev");
  apply_ln("readlink");
  apply_ln("realpath");
  apply_ln("renice");
  apply_ln("reset");
  apply_ln("rm");
  apply_ln("rmdir");
  apply_ln("rmmod");
  apply_ln("run-parts");
  apply_ln("sed");
  apply_ln("seq");
  apply_ln("setsid");
  apply_ln("sh");
  apply_ln("sha1sum");
  apply_ln("sha256sum");
  apply_ln("sha512sum");
  apply_ln("sleep");
  apply_ln("sort");
  apply_ln("split");
  apply_ln("stat");
  apply_ln("strings");
  apply_ln("stty");
  apply_ln("swapoff");
  apply_ln("swapon");
  apply_ln("sync");
  apply_ln("sysctl");
  apply_ln("tac");
  apply_ln("tail");
  apply_ln("tar");
  apply_ln("tee");
  apply_ln("test");
  apply_ln("time");
  apply_ln("top");
  apply_ln("touch");
  apply_ln("tr");
  apply_ln("true");
  apply_ln("tty");
  apply_ln("umount");
  apply_ln("uname");
  apply_ln("uniq");
  apply_ln("unix2dos");
  apply_ln("unlzop");
  apply_ln("unzip");
  apply_ln("uptime");
  apply_ln("usleep");
  apply_ln("uudecode");
  apply_ln("uuencode");
  apply_ln("watch");
  apply_ln("wc");
  apply_ln("which");
  apply_ln("whoami");
  apply_ln("xargs");
  apply_ln("yes");
  apply_ln("zcat");

  if (mode>0) {
    ui_print("Removing some toolbox commands");
    apply_rm("cat");
    apply_rm("chmod");
    apply_rm("chown");
    apply_rm("dd");
    apply_rm("ln");
    apply_rm("ls");
    apply_rm("mkdir");
    apply_rm("mv");
    apply_rm("rm");
    apply_rm("rmdir");
    if (mode>1) {
      ui_print("Removing even more toolbox commands");
      apply_rm("cmp");
      apply_rm("date");
      apply_rm("df");
      apply_rm("dmesg");
      apply_rm("id");
      apply_rm("insmod");
      apply_rm("kill");
      apply_rm("lsmod");
      apply_rm("mount");
      apply_rm("printenv");
      apply_rm("ps");
      apply_rm("renice");
      apply_rm("sleep");
      apply_rm("sync");
      apply_rm("top");
      apply_rm("umount");
    }
  }
  ui_print("Done!\n");
}

static char* startval[] = {"DATA_FS=","CACHE_FS=","DBDATA_FS=","DATA_LOOP=","CACHE_LOOP=","DBDATA_LOOP=","BIND_DATA_TO_DBDATA=", NULL};
static int catvals[] = { 3,6,7,999 };
static char* categories[][5] = {
    {"rfs","jfs","ext4",NULL,NULL},
    {"false","ext2",NULL,NULL},
    {"false","data",NULL,NULL},
    {NULL,NULL,NULL,NULL}
  };



void advanced_lagfix_menu() {
  static char* headers[] = { "Advanced Lagfix Menu",
                             "", NULL };

  int chosen_item = 0;
  for (;;)
    {
        char* list[8];
        FILE* f = fopen("/system/etc/lagfix.conf","r");
        int i=0;
        for (i=0;i<7;i++) {
          list[i] = malloc(64);
        }
        list[7] = NULL;
        if (f==NULL) {
          LOGE("Could not open lagfix.conf!\n");
        } else {
          ui_print("Current configuration is:\n");
          i=0;
          while ((i<7) && (fgets(list[i],63,f))) {
            ui_print(list[i]);
            list[i][strlen(list[i])-1]='\0'; // remove trailing newline
            i++;
          }
          fclose(f);
        }

        chosen_item = get_menu_selection(headers, list, 0);
        if (chosen_item == GO_BACK)
            break;

        int category=0;
        while (chosen_item>=catvals[category]) {
          category++;
        }

        ui_print("%d\n",category);

        int curval=0;
        while ((categories[category][curval]) && (memcmp(list[chosen_item]+strlen(startval[chosen_item]),categories[category][curval],strlen(categories[category][curval]))!=0))
          curval++;

        ui_print("%d\n",curval);

        if (categories[category][curval]) {
          curval++;
          if (!categories[category][curval]) {
            curval = 0;
          }
        } else {
          curval=0;
        }

        ui_print("%d\n",curval);

        sprintf(list[chosen_item],"%s%s",startval[chosen_item],categories[category][curval]);

        f = fopen("/system/etc/lagfix.conf","w+");
        for (i=0; i<7; i++) {
          fprintf(f,"%s\n",list[i]);
        }
        fclose(f);
    }
}

void lagfix_menu() {
  static char* headers[] = {  "Lagfix Menu & Advance Settings",
                              NULL
  };

  static char* list[] = { "Enable Stock RFS ( Disable Lagfix )",
                          "Enable Voodoo",
                          "Enable JFS",
                          "Advanced options",
                          NULL
    };

    for (;;)
    {
        FILE* f = fopen("/system/etc/lagfix.conf","r");
        if (f==NULL) {
          LOGE("Could not open lagfix.conf!");
        } else {
          char buf[64];
          ui_print("\n\nCurrent configuration is:\n");
          while (fgets(buf,63,f)) {
            ui_print(buf);
          }
          fclose(f);
        }

        int chosen_item = get_menu_selection(headers, list, 0);
        if (chosen_item == GO_BACK)
            break;
        switch (chosen_item)
        {
          case 0:f = fopen("/system/etc/lagfix.conf","w+");fprintf(f,"DATA_FS=rfs\nCACHE_FS=rfs\nDBDATA_FS=rfs\nDATA_LOOP=false\nCACHE_LOOP=false\nDBDATA_LOOP=false\nBIND_DATA_TO_DBDATA=false\n");fclose(f);break;
          case 1:f = fopen("/system/etc/lagfix.conf","w+");fprintf(f,"DATA_FS=ext4\nCACHE_FS=ext4\nDBDATA_FS=ext4\nDATA_LOOP=false\nCACHE_LOOP=false\nDBDATA_LOOP=false\nBIND_DATA_TO_DBDATA=false\n");fclose(f);break;
          case 2:f = fopen("/system/etc/lagfix.conf","w+");fprintf(f,"DATA_FS=jfs\nCACHE_FS=jfs\nDBDATA_FS=jfs\nDATA_LOOP=false\nCACHE_LOOP=false\nDBDATA_LOOP=false\nBIND_DATA_TO_DBDATA=false\n");fclose(f);break;
          case 3:advanced_lagfix_menu(); break;
        }
    }
}

int searchfor_in_config_file(const char* searchfor, int category) {
  int res = -1;
  char buf[64]; 
  FILE *f = fopen("/system/etc/lagfix.conf.old","r");
  if (!f) return -1;
  while (fgets(buf,63,f)) {
    if (memcmp(searchfor,buf,strlen(searchfor))==0) {
      res++;
      while (categories[category][res] && (memcmp(buf+strlen(searchfor),categories[category][res],strlen(categories[category][res]))!=0)) {
        res++;
      }
      if (!categories[category][res]) res=-1;
    }
  }
  fclose(f);
  return res;
}

int get_loop_options(const char * name) {
  char searchfor[64];
  sprintf(searchfor,"%s_LOOP=",name);
  return searchfor_in_config_file(searchfor,1);
}

int get_fs_options(const char * name) {
  char searchfor[64];
  sprintf(searchfor,"%s_FS=",name);
  return searchfor_in_config_file(searchfor,0);
}

int dont_use_bind_options = 0;
int get_bind_options() {
  if (dont_use_bind_options) return 0;
  return searchfor_in_config_file("BIND_DATA_TO_DBDATA=",2);
}

void mount_block(const char* name, const char* blockname, const char* loopblock, const char* destnoloop, const char* destloop) {
  int getfsopts,getloopopts,bindopts;
  getfsopts = get_fs_options(name);
  getloopopts = get_loop_options(name);
  char tmp[256];
  if (getloopopts>0) {
    sprintf(tmp,"mkdir %s",destloop);__system(tmp);
    sprintf(tmp,"chmod 700 %s",destloop);__system(tmp);
    if (getfsopts==0) {
      sprintf(tmp,"mount -t rfs -o nosuid,nodev,check=no %s %s",blockname,destloop);
    } else if (getfsopts==1) {
      sprintf(tmp,"mount -t jfs -o noatime,nodiratime %s %s",blockname,destloop);
    } else if (getfsopts==2) {
      sprintf(tmp,"mount -t ext4 -o noatime,barrier=0,noauto_da_alloc %s %s",blockname,destloop);
    } else if (getfsopts==3) {
      sprintf(tmp,"mount -t ext2 -o noatime,nodiratime %s %s",blockname,destloop);
    } else if (getfsopts==-1) {
      // rfs can't autodetect itself
      sprintf(tmp,"mount -t rfs -o nosuid,nodev,check=no %s %s",blockname,destnoloop);
      sprintf(tmp,"mount %s %s",blockname,destloop);
    }
    __system(tmp);
    sprintf(tmp,"losetup %s %s/.extfs",loopblock,destloop);__system(tmp);
    sprintf(tmp,"mount -t ext2 %s %s",loopblock,destnoloop);__system(tmp);
  } else {
    if (getfsopts==0) {
      sprintf(tmp,"mount -t rfs -o nosuid,nodev,check=no %s %s",blockname,destnoloop);
    } else if (getfsopts==1) {
      sprintf(tmp,"mount -t jfs -o noatime,nodiratime %s %s",blockname,destnoloop);
    } else if (getfsopts==2) {
      sprintf(tmp,"mount -t ext4 -o noatime,barrier=0,noauto_da_alloc %s %s",blockname,destnoloop);
    } else if (getfsopts==3) {
      sprintf(tmp,"mount -t ext2 -o noatime,nodiratime %s %s",blockname,destnoloop);
    } else if (getfsopts==-1) {
      // rfs can't autodetect itself
      sprintf(tmp,"mount -t rfs -o nosuid,nodev,check=no %s %s",blockname,destnoloop);
      sprintf(tmp,"mount %s %s",blockname,destnoloop);
    }
    __system(tmp);
  }
}

// there should be some checks whether the action actually succeded
int ensure_lagfix_mount_points(const RootInfo *info) {
  int bindopts;
  bindopts = get_bind_options();
  if (strcmp(info->name,"DATA:")==0) {
    mount_block("DATA","/dev/block/mmcblk0p2","/dev/block/loop1","/data","/res/odata");
    if (bindopts>0) {
      ensure_root_path_mounted("DATADATA:");
      __system("mkdir -p /dbdata/.data/data");
      __system("mkdir -p /data/data");
      __system("mount -o bind /dbdata/.data/data /data/data");
    }
  } else if (strcmp(info->name,"DATADATA:")==0) {
    mount_block("DBDATA","/dev/block/stl10","/dev/block/loop2","/dbdata","/res/odbdata");
  } else if (strcmp(info->name,"CACHE:")==0) {
    mount_block("CACHE","/dev/block/stl11","/dev/block/loop3","/cache","/res/ocache");
  } else {
    return 1;
  }
  return 0;
}

// not implemented
int ensure_lagfix_unmount_points(const RootInfo *info) {
  if (strcmp(info->name,"DATA:")==0) {
    return -1;
  } else if (strcmp(info->name,"DATADATA:")==0) {
    return -1;
  } else if (strcmp(info->name,"CACHE:")==0) {
    return -1;
  } else return 1;
}

int ensure_lagfix_formatted(const RootInfo *info) {
  // we won't remove hidden files in root yet
  if (strcmp(info->name,"DATA:")==0) {
    __system("rm -rf /data/*");
    __system("rm -rf /data/.*");
    return 0;
  } else if (strcmp(info->name,"DATADATA:")==0) {
    __system("rm -rf /dbdata/*");
    if (!get_bind_options()) {
      __system("rm -rf /dbdata/.*");
    }
    return 0;
  } else if (strcmp(info->name,"CACHE:")==0) {
    __system("rm -rf /cache/*");
    __system("rm -rf /cache/.*");
    return 0;
  } else return 1;
}

int unmount_all_lagfixed() {
  sync();
  __system("umount -f /data/data");
  __system("umount -f /data/dalvik-cache");
  __system("umount -f -d /dev/block/loop3");
  __system("umount -f -d /dev/block/loop2");
  __system("umount -f -d /dev/block/loop1");
  __system("umount -f /res/ocache");
  __system("umount -f /res/odata");
  __system("umount -f /res/odbdata");
  __system("umount -f /cache");
  __system("umount -f /data");
  __system("umount -f /dbdata");
  return 0;
}

int create_lagfix_partition(int id) {
  char loopname[64],blockname[64];
  char looppos[64],blockpos[64];
  char name[64];
  int loopsize;
  if (id==0) {
    strcpy(loopname,"/dev/block/loop1");
    strcpy(blockname,"/dev/block/mmcblk0p2");
    strcpy(looppos,"/res/odata");
    strcpy(blockpos,"/data");
    strcpy(name,"DATA");
    loopsize = 1831634944;
  } else if (id==1) {
    strcpy(loopname,"/dev/block/loop2");
    strcpy(blockname,"/dev/block/stl10");
    strcpy(looppos,"/res/odbdata");
    strcpy(blockpos,"/dbdata");
    strcpy(name,"DBDATA");
    loopsize = 128382976;
  } else {
    strcpy(loopname,"/dev/block/loop3");
    strcpy(blockname,"/dev/block/stl11");
    strcpy(looppos,"/res/ocache");
    strcpy(blockpos,"/cache");
    strcpy(name,"CACHE");
    loopsize = 29726720;
  }
  int ft = get_fs_options(name);
  int loop = get_loop_options(name);
  char tmp[256];
  if (ft==0) {
    if (id==0) {
      sprintf(tmp,"/sbin/fat.format -l %s -F 32 -S 4096 -s 4 %s",name,blockname);
      // we can't create small partitions that are valid as rfs with fat.format, so we'll use some compressed pre-made valid rfs images
    } else {
      // EDIT: it seems we can
      sprintf(tmp,"/sbin/fat.format %s",blockname);
      /*
      if (id==1) {
        sprintf(tmp,"gunzip -c /res/misc/dbdata.rfs.gz | dd of=%s",blockname);
      } else {
        sprintf(tmp,"gunzip -c /res/misc/cache.rfs.gz | dd of=%s",blockname);
      }*/
    }
    __system(tmp);
  } else if (ft==1) {
    sprintf(tmp,"/sbin/mkfs.jfs -L %s %s",name,blockname);
    __system(tmp);
  } else if (ft==2) {
    sprintf(tmp,"/sbin/mkfs.ext4 -L %s -b 4096 -m 0 -F %s",name,blockname);
    __system(tmp);
  } else if (ft==3) {
    sprintf(tmp,"/sbin/mkfs.ext2 -L %s -b 4096 -m 0 -F %s",name,blockname);
    __system(tmp);
  }

  if (loop) {
    sprintf(tmp,"mount %s %s",blockname,looppos);
    __system(tmp);
    sprintf(tmp,"%s/.extfs",looppos);
    FILE*f = fopen(tmp,"w+");fclose(f);
    truncate(tmp,loopsize);
    sprintf(tmp,"losetup /dev/block/loop0 %s/.extfs",looppos);
    __system(tmp);
    __system("/sbin/mkfs.ext2 -b 4096 -m 0 -F /dev/block/loop0");
    __system("losetup -d /dev/block/loop0");
    sprintf(tmp,"umount %s",blockname);
    __system(tmp);
  }
  return 0;
}

void ui_print_file(char* name) {
  FILE *f = fopen(name,"r");
  char tmp[64];
  if (f) {
    while (fgets(tmp,63,f)) {
      ui_print(tmp);
    }
    fclose(f);
  }
}

int do_lagfix(int do_fr) {
  ui_print("old config was:\n");
  ui_print_file("/system/etc/lagfix.conf.old");
  ui_print("new config is:\n");
  ui_print_file("/system/etc/lagfix.conf");
  ui_print("checking mounts available\n");
  if (ensure_root_path_mounted("DATA:")!=0) return -1;
  if (ensure_root_path_mounted("DATADATA:")!=0) return -1;
  if (ensure_root_path_mounted("CACHE:")!=0) return -1;
  if (ensure_root_path_mounted("SDCARD:")!=0) return -1;

  char tmp[PATH_MAX];
  nandroid_generate_timestamp_path(tmp);
  if (do_fr!=2) {
    ui_print("Creating a nandroid backup at %s\n",tmp);
    if (nandroid_backup_flags(tmp,DONT_BACKUP_SYSTEM)!=0) return -1;
  } else {
    ui_print("Not creating a backup\n");
  }

  ui_print("Backup completed, recreating file systems\n");

  ui_print("Unmounting\n");
  unmount_all_lagfixed();

  ui_print("Switching to new config\n");
  __system("cp /system/etc/lagfix.conf /system/etc/lagfix.conf.old");

  ui_print("Creating /data\n");
  create_lagfix_partition(0);
  ui_print("Creating /dbdata\n");
  create_lagfix_partition(1);
  ui_print("Creating /cache\n");
  create_lagfix_partition(2);

  ui_print("Mounting to test\n");
  dont_use_bind_options = 1;
  if (ensure_root_path_mounted("DATA:")!=0) return -1;
  if (ensure_root_path_mounted("DATADATA:")!=0) return -1;
  if (ensure_root_path_mounted("CACHE:")!=0) return -1;
  dont_use_bind_options = 0;
  __system("mount"); // for debug purposes
  if (get_bind_options()) {
    ui_print("Creating bind directories\n");
    __system("mkdir -p /dbdata/.data/data");
    __system("mkdir -p /data/data");
  }

  ui_print("Unmounting again\n");
  unmount_all_lagfixed();

  if (do_fr) {
    ui_print("Factory reset was requested, not restoring backed up data\n");
  } else {
    ui_print("Restoring data\n");
    nandroid_restore(tmp,0,0,1,1,0);
    // restore might have brought some .data into dbdata, clear them
    if (!get_bind_options()) {
      if (ensure_root_path_mounted("DATADATA:")!=0) return -1;
      __system("rm -rf /dbdata/.data");
    }
  }

  __system("mount");
  ui_print("Unmounting again to be sure\n");
  sync();
  sleep(5);
  unmount_all_lagfixed();
  sync();
  return 0;
}

int lagfixer_main(int argc, char** argv) {
  ui_init();
  ui_print(EXPAND(RECOVERY_VERSION)" - lagfixer\n");
  create_fstab();
  ui_set_show_text(1);

  int res;
  int opts = 0;
  if ((argc>=2)&&(strcmp(argv[1],"fr")==0)) {
    opts=2;
  } else if ((argc>=2)&&(strcmp(argv[1],"b")==0)) {
    opts=1;
  }
  res = do_lagfix(opts);
  if (res) {
    ui_print("Something went wrong while doing the lagfix, sorry.\n");
  } else {
    ui_print("Done. Your device will reboot soon or enter recovery mode to debug.\n");
  }
  sleep(5);

  gr_exit();
  ev_exit();
  return 0;
}

void tweak_menu() {
    static char* headers[] = {  "Start-up tweaks config",
                                "",
                                NULL
    };

    static char* list[] = { "IO scheduler",
                            "Kernel VM management",
                            "Kernel scheduler",
                            "Misc tweaks",
                            NULL
    };

    const int numtweaks = 4;
    static char* options[] = { "IOSCHED","KERNELVM","KERNELSCHED","MISC" };

    int tweaks[numtweaks];
    int i;
    char buf[128];
    for (;;)
    {
        for (i=0;i<numtweaks;i++) tweaks[i]=0;
        FILE* f = fopen("/system/etc/tweaks.conf","r");
        ui_print("\n\nEnabled options:\n");
        if (f) {
          while (fgets(buf,127,f)) {
            ui_print(buf);
            for (i=0; i<numtweaks; i++) {
              if (memcmp(buf,options[i],strlen(options[i]))==0) tweaks[i]=1;
            }
          }
          fclose(f);
        }
        int chosen_item = get_menu_selection(headers, list, 0);
        if (chosen_item == GO_BACK)
            break;
        tweaks[chosen_item] = tweaks[chosen_item]?0:1;
        f = fopen("/system/etc/tweaks.conf","w+");
        if (f) {
          for (i=0; i<numtweaks; i++) {
            if (tweaks[i]) fprintf(f,"%s\n",options[i]);
          }
          fclose(f);
        } else {
          ui_print("Could not create config file\n");
        }
    }

}

void bln_menu() {
    static char* headers[] = {  "Back Light Notification Menu",
                                "",
                                NULL
    };

    static char* list[] = { "Turn BLN On",
                            "Turn BLN Off",
                            NULL
    };

    for (;;)
    {
	unsigned int bln_enabled = 0;
	FILE* f = fopen("/system/etc/bln.conf","r");
	if (f)
	    fscanf (f,"%u",&bln_enabled);
	ui_print("\n\nBLN is now: %s\n", bln_enabled? "enabled":"disabled");
	int chosen_item = get_menu_selection(headers, list, 0);
	if (chosen_item == GO_BACK)
	    break;
	switch (chosen_item)
	{
	    case 0:
		bln_enabled = 1;
		break;
	    case 1:
		bln_enabled = 0;
		break;
	}
	
	f = fopen("/system/etc/bln.conf","w+");
	if (f) {
	    fprintf(f,"%u\n", bln_enabled);
	    fclose(f);
	} else {
	    ui_print("Could not create config file\n");
	}
}

}

void apply_root_menu() {
    static char* headers[] = {  "Install superuser",
                                "",
                                NULL
    };

    static char* list[] = { "Simple: install busybox+su",
                            "Adv: also rm some toolbox cmds",
                            "Ext: also rm most toolbox cmds",
                            NULL
    };

    for (;;)
    {
        int chosen_item = get_menu_selection(headers, list, 0);
        if (chosen_item == GO_BACK)
            break;
        switch (chosen_item)
        {
            case 0:
              if (confirm_selection("Confirm root","Yes - apply root to device")) {
                apply_root_to_device(0);
              }
              break;
            case 1:
              if (confirm_selection("Confirm root","Yes - apply root to device")) {
                apply_root_to_device(1);
              }
              break;
            case 2:
              if (confirm_selection("Confirm root","Yes - apply root to device")) {
                apply_root_to_device(2);
              }
              break;
        }
    }

}


void show_advanced_lfs_menu() {
    static char* headers[] = {  "ULF Settings  Menu",
                                "",
                                NULL
    };

    static char* list[] = { "Reboot into Download",
                            "Install Superuser",
                            "Change Lagfix Settings",
                            "Tweak options",
                            "Back Light Notification",
                            "Set default permissions",
                            NULL
    };

    static char* headersno[] = { "Universal Lagfix Kernel Not Found",
                                 "These options only work if ULK is used",
                                 "",
                                 NULL };
    static char* listno[] = { "Okay", NULL };

    struct stat ss;
    // we check for the pre-init.log, which this kernel creates each run
    if (stat("/res/pre-init.log",&ss)!=0) {
      get_menu_selection(headersno,listno,0);
    } else {
      for (;;)
      {
          int chosen_item = get_menu_selection(headers, list, 0);
          if (chosen_item == GO_BACK)
              break;
          switch (chosen_item)
          {
              case 0:
                  __reboot(LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART2, "download");
                  break;
              case 1:
              {
                apply_root_menu();
                break;
              }
              case 2:
              {
                lagfix_menu();
                break;
              }
              case 3:
              {
                tweak_menu();
                break;
              }
              case 4:
              {
                bln_menu();
                break;
              }
              case 5:
              {
                if (confirm_selection("Confirm setting permissions","Yes - run fix_permissions_sgs")) {
                  ensure_root_path_mounted("SYSTEM:");
                  ensure_root_path_mounted("DATA:");
                  ensure_root_path_mounted("DATADATA:");
                  ensure_root_path_mounted("CACHE:");
                  ui_print("Starting fixing\n");
                  __system("/sbin/fix_permissions_sgs");
                  ui_print("Done\n");
                }
              }
          }
      }
    }
}
