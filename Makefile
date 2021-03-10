iso := build/infos.iso
grub_cfg := boot/grub.cfg
kernel := infos/out/infos-kernel
kernel_src := infos/

.PHONY: all clean iso

all: iso

clean:
	@rm -r build
	make -C $(kernel_src) clean

kernel:
	make -C $(kernel_src)

iso: kernel $(iso)

$(iso): $(grub_cfg) kernel
	mkdir -p build/isofiles/boot/grub
	cp $(kernel) build/isofiles/boot/infos-kernel
	cp $(grub_cfg) build/isofiles/boot/grub
	grub-mkrescue -o $(iso) build/isofiles 2> /dev/null
	rm -r build/isofiles
