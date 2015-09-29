#include <common.h>
#include <linux/mtd/mtd.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <jffs2/jffs2.h>
#include <nand.h>

#define NAND_ART_PART_NAME              "art"
#define NAND_ART_SIZE			(64 * 1024) /*64kb*/

static char* nand_art_cache = NULL;

nand_art_get_cal_data(char **art_buff)
{
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;

	loff_t off = 0;
	size_t size = 0;
	loff_t maxsize = 0;

	if (nand_art_cache) {
		*art_buff = nand_art_cache;
		return 0;
	}

	ret = mtdparts_init();
	if (ret)
		return ret;

	ret = find_dev_and_part(NAND_ART_PART_NAME, &dev, &pnum, &part);
	if (ret)
		return ret;
	if (dev->id->type != MTD_DEV_TYPE_NAND) {
		puts("not a NAND device\n");
		return -1;
	}

	off = part->offset;
	nand_info_t *nand = &nand_info[nand_curr_device];
	maxsize = nand_info[nand_curr_device].size - off;

	u_char* art = NULL;
	art = malloc(NAND_ART_SIZE);
	if (!art){
		return -2;
	}

	memset(art, 0, NAND_ART_SIZE);
	size=NAND_ART_SIZE;
	ret = nand_read_skip_bad(nand, off, &size, NULL, maxsize, art);

	if (!ret) {
		nand_art_cache = art;
		*art_buff = nand_art_cache;
	}
	return ret;
}
