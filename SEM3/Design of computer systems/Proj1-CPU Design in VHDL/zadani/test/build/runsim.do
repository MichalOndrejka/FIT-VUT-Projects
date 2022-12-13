onerror {
	quit -f -code 1
}

vmap -c
if [file exists build/work] {vdel -lib build/work -all}
vlib build/work

vmap work build/work

vcom -work work +acc "$::env(VHDL_SRC)"

vsim  -onfinish exit -foreign "cocotb_init $::env(QUESTA_FLI)" work.cpu

onbreak resume

add wave sim:/cpu/*

run -all
#quit
