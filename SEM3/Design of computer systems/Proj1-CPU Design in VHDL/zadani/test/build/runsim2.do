onerror {
	quit -f -code 1
}

vmap -c
if [file exists build/work2] {vdel -lib build/work2 -all}
vlib build/work2

vmap work build/work2

vcom -work work +acc "$::env(VHDL_SYNTH_SRC)"

vsim  -onfinish exit -foreign "cocotb_init $::env(QUESTA_FLI)" work.cpu

onbreak resume

add wave sim:/cpu/*

run -all
#quit
