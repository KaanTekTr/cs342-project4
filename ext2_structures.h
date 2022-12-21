#include <stdint.h>
#include <linux/types.h>

#define EXT_NAME_LEN 255
#define EXT_N_BLOCKS 15
#define EXT_NDIR_BLOCKS 12

struct ext2_dir_entry {
    __u32 inode; // inode number
    __u16 rec_len; // directory entry length
    __u8 name_len; // name length
    __u8 file_type; //
    char name[EXT_NAME_LEN]; // file name
};

struct ext2_inode {
    __le16 i_mode;
    __le16 i_uid;
    __le32 i_size;
    __le32 i_atime;
    __le32 i_ctime;
    __le32 i_mtime;
    __le32 i_dtime;
    __le16 i_gid;
    __le16 i_links_count;
    __le32 i_blocks;
    __le32 i_flags;
    __le32 i_osd1;
    __le32 i_block[EXT_N_BLOCKS];
    __le32 i_generation;
    __le32 i_file_acl;
    __le32 i_dir_acl;
    __le32 i_faddr;
    __le32 i_osd2[3];
};

struct ext2_group_desc
{
    __le32 bg_block_bitmap; /* Blocks bitmap block */
    __le32 bg_inode_bitmap; /* Inodes bitmap block */
    __le32 bg_inode_table; /* Inodes table block */
    __le16 bg_free_blocks_count; /* Free blocks count */
    __le16 bg_free_inodes_count; /* Free inodes count */
    __le16 bg_used_dirs_count; /* Directories count */
    __u16 bg_pad;
    __le32 bg_reserved[3];
};