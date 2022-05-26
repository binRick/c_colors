
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
TIDIED_FILES = $(M1_DIR)/src/*.c $(M1_DIR)/include/*.h
##############################################################
CD_LOADER = cd $(LOADER_DIR)
CD_PROJECT = cd $(PROJECT_DIR)
CD_M1 = cd $(M1_DIR)
##############################################################
##############################################################

all: ensure dirs 
	@make do-m1

do-build: do-m1-test

MESON_DIRS = m1

clean: 
	@rm -rf $(EMBEDS_DIR) build

ensure: dirs-embeds

setup:
	@clib i

dirs: ensure dirs-embeds 

dirs-embeds:
	@mkdir -p $(EMBEDS_DIR)

do-m1: ensure do-m1-test

do-m1-meson: 
	@eval $(CD_M1) && { meson build || { meson build --reconfigure || { meson build --wipe; } && meson build; }; }

do-m1-build: do-m1-meson
	@eval $(CD_M1) && { ninja -C build; }

do-m1-test: do-m1-build
	@eval $(CD_M1) && { ninja test -C build -v; }

test: do-m1-test

uncrustify:
	@$(UNCRUSTIFY) -c etc/uncrustify.cfg --replace $(TIDIED_FILES) 
	@shfmt -w scripts/*.sh

uncrustify-clean:
	@find $(TIDIED_FILES) vendor -type f -name "*unc-back*"|xargs -I % unlink %

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
	@$(PASSH) -L .nodemon.log $(NODEMON) -w '*/meson.build' --delay 1 -i '*/subprojects' -I  -w 'include/*.h' -w meson.build -w src -w Makefile -w loader/meson.build -w loader/src -w loader/include -i '*/embeds/*' -e tpl,build,sh,c,h,Makefile -x env -- bash -c 'make dev-all||true'


