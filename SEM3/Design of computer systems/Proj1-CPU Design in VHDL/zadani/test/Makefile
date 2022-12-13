.EXPORT_ALL_VARIABLES:

GHDL_BINARY ?= /usr/local/share/ghdl/bin/ghdl
ifeq ("$(wildcard $(GHDL_BINARY))","")
    GHDL_BINARY = ghdl
endif

VSIM_BINARY ?= /mnt/data/tools/questa/questasim.2022/bin/vsim
ifeq ("$(wildcard $(VSIM_BINARY))","")
    VSIM_BINARY = vsim
endif

GHDL_VPI = $(shell cocotb-config --lib-name-path vpi ghdl)
QUESTA_FLI = $(shell cocotb-config --lib-name-path fli questa)

WORK_DIR=build

MODULE=cpu
GPI_EXTRA=

TOPLEVEL=cpu
VHDL_SRC=../src/cpu.vhd

VHDL_SYNTH_SRC=$(WORK_DIR)/cpu_synth.vhd

ifeq ($(OS),Windows_NT)
PYTHONHOME=$(shell python -c "import sys; print(sys.prefix)")
else
LIBPYTHON_LOC=$(shell cocotb-config --libpython)
endif 

all:	ghdl

clean:
	$(GHDL_BINARY) --clean --workdir=$(WORK_DIR) --work=work
	$(GHDL_BINARY) --clean --workdir=$(WORK_DIR) --work=work2
    

ghdl:	$(VHDL_SRC)
	$(GHDL_BINARY) -i --ieee=synopsys -fexplicit --workdir=$(WORK_DIR) --work=work $(VHDL_SRC)
	$(GHDL_BINARY) -m --ieee=synopsys -fexplicit --workdir=$(WORK_DIR) -P$(WORK_DIR) --work=work $(TOPLEVEL)
	$(GHDL_BINARY) -r --ieee=synopsys -fexplicit --workdir=$(WORK_DIR) -P$(WORK_DIR) --work=work $(TOPLEVEL) --vpi=$(GHDL_VPI)
    

$(VHDL_SYNTH_SRC):	$(VHDL_SRC)
	$(GHDL_BINARY) -i --ieee=synopsys -fexplicit --workdir=$(WORK_DIR) --work=work $(VHDL_SRC)
	$(GHDL_BINARY) -m --ieee=synopsys -fexplicit --workdir=$(WORK_DIR) -P$(WORK_DIR) --work=work $(TOPLEVEL)
	$(GHDL_BINARY) --synth --latches --ieee=synopsys -fexplicit --workdir=$(WORK_DIR) --out=raw-vhdl --work=work $(TOPLEVEL) >$(VHDL_SYNTH_SRC)

synthesized:	$(VHDL_SYNTH_SRC)
# synthesize the module
# simulate the synthesized module
	$(GHDL_BINARY) -i --ieee=synopsys -fexplicit --workdir=$(WORK_DIR) --work=work2 $(VHDL_SYNTH_SRC)
	$(GHDL_BINARY) -m --ieee=synopsys -fexplicit --workdir=$(WORK_DIR) -P$(WORK_DIR) --work=work2 $(TOPLEVEL)
	$(GHDL_BINARY) -r --ieee=synopsys -fexplicit --workdir=$(WORK_DIR) -P$(WORK_DIR) --work=work2 $(TOPLEVEL) --vpi=$(GHDL_VPI)


LM_LICENSE_FILE=17170@semik.fit.vutbr.cz

questa:
	$(VSIM_BINARY) -gui -64 -do ./build/runsim.do

questa-synthesized:	$(VHDL_SYNTH_SRC)
	$(VSIM_BINARY) -gui -64 -do ./build/runsim2.do
