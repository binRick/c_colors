
default: all
##############################################################
PASSH=$(shell command -v passh)
GIT=$(shell command -v git)
SED=$(shell command -v gsed||command -v sed)
NODEMON=$(shell command -v nodemon)
FZF=$(shell command -v fzf)
BLINE=$(shell command -v bline)
UNCRUSTIFY=$(shell command -v uncrustify)
PWD=$(shell command -v pwd)
FIND=$(shell command -v find)
##############################################################
DIR=$(shell $(PWD))
M1_DIR=$(DIR)
LOADER_DIR=$(DIR)/loader
EMBEDS_DIR=$(DIR)/embeds
VENDOR_DIR=$(DIR)/vendor
PROJECT_DIR=$(DIR)
MESON_DEPS_DIR=$(DIR)/meson/deps
VENDOR_DIR=$(DIR)/vendor
DEPS_DIR=$(DIR)/deps
ETC_DIR=$(DIR)/etc
DOCKER_DIR=$(DIR)/docker
##############################################################
TIDIED_FILES = \
			   colors-csv-parser/*.c colors-csv-parser/*.h \
			   rgb-ansi-utils/*.c rgb-ansi-utils/*.h \
			   colors-json-parser/*.c colors-json-parser/*.h \
			   db/*.h db/*.c \
			   dbmem*/*.h dbmem*/*.c \
			   parse-colors/*.c parse-colors/*.h
##############################################################
CD_LOADER = cd $(LOADER_DIR)
CD_PROJECT = cd $(PROJECT_DIR)
CD_M1 = cd $(M1_DIR)
##############################################################

all: ensure dirs do-hex-png-pixel-test

do-write-etc-color-names:
	@./build/parse-colors/parse-colors -c 50000 -o ./etc/parsed-colors.json

clean: 
	@rm -rf $(EMBEDS_DIR) build
	@rm -rf *.png

ensure: dirs-embeds

setup:
	@clib i

dirs: ensure dirs-embeds 

dirs-embeds:
	@mkdir -p $(EMBEDS_DIR)

do-hex-png-pixel-meson: 
	@meson build || { meson build --reconfigure || { meson build --wipe; } && meson build; }

do-hex-png-pixel-build: do-hex-png-pixel-meson
	@eval $(CD_M1) && { ninja -C build; }

do-hex-png-pixel-test: do-hex-png-pixel-build
	@ninja test -C build -v

do-m1-meson: 
	@eval $(CD_M1) && { meson build || { meson build --reconfigure || { meson build --wipe; } && meson build; }; }

do-m1-build: do-m1-meson
	@eval $(CD_M1) && { ninja -C build; }

do-m1-test: do-m1-build
	@eval $(CD_M1) && { ninja test -C build -v; }

test: do-hex-png-pixel-test

uncrustify:
	@$(UNCRUSTIFY) -c submodules/meson_deps/etc/uncrustify.cfg --replace $(TIDIED_FILES) 

uncrustify-clean:
	@find  . -type f -name "*unc-back*"|xargs -I % unlink %

fix-dbg:
	@$(SED) 's|, % s);|, %s);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % lu);|, %lu);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % d);|, %d);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % zu);|, %zu);|g' -i $(TIDIED_FILES)

tidy: uncrustify uncrustify-clean fix-dbg

dev-all: all

pull:
	@git pull

dev-clean: clean dev

dev: pull tidy nodemon

dev-loader:
	@$(PASSH) -L .nodemon.log $(NODEMON) -w '*/meson.build' --delay 1 -i '*/subprojects' -I  -w 'include/*.h' -w meson.build -w src -w Makefile -w loader/meson.build -w loader/src -w loader/include -i '*/embeds/*' -e tpl,build,sh,c,h,Makefile -x env -- bash -c 'make do-loader||true'

nodemon:
	@$(PASSH) -L .nodemon.log $(NODEMON) -w '*/meson.build' -w "*/*.c" -w "*/*.h" --delay 1 -i '*/subprojects' -I  -w 'include/*.h' -w meson.build -w src -w Makefile -w loader/meson.build -w loader/src -w loader/include -i '*/embeds/*' -e tpl,build,sh,c,h,Makefile -x env -- bash -c 'make dev-all||true'


git-pull:
	@git pull --recurse-submodules
git-submodules-pull-master:
	@git submodule foreach git pull origin master --jobs=10
git-submodules-update:
	@git submodule update --init	

meson-binaries:
	@meson introspect --targets  meson.build -i | jq 'map(select(.type == "executable").filename)|flatten|join("\n")' -Mrc

