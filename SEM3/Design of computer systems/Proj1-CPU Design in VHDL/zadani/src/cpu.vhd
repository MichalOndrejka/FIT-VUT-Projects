-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2022 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xondre15 <login AT stud.fit.vutbr.cz>
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 signal cnt_reg : std_logic_vector (11 downto 0); 
 signal cnt_inc : std_logic;
 signal cnt_dec : std_logic;

 signal pc_reg  : std_logic_vector(12 downto 0) := "0000000000000";
 signal pc_inc  : std_logic;
 signal pc_dec  : std_logic;

 signal ptr_reg : std_logic_vector(12 downto 0) := "1000000000000";
 signal ptr_inc : std_logic;
 signal ptr_dec : std_logic; 

 signal mx1_sel : std_logic; 
 signal mx2_sel : std_logic_vector(1 downto 0);

 type STATE_TYPE is (
  idle, fetch, decode,
  actualize_addr,
  state_ptr_inc, state_ptr_dec,
  state_value_inc1, state_value_inc2, state_value_inc3, state_value_inc4,
  state_value_dec1, state_value_dec2, state_value_dec3, state_value_dec4,
  state_while_start1, state_while_start2, state_while_start3, state_while_start4,
  state_while_end1, state_while_end2, state_while_end3, state_while_end4, state_while_end5, state_while_end6, state_while_end7, state_while_end8,
  state_dowhile_start,
  state_dowhile_end1, state_dowhile_end2, state_dowhile_end3, state_dowhile_end4, state_dowhile_end5, state_dowhile_end6, state_dowhile_end7, state_dowhile_end8,
  state_print1, state_print2, state_print3,
  state_load1, state_load2, 
  state_null
 );
 signal pstate : STATE_TYPE := idle;
 signal nstate : STATE_TYPE;

 signal c : std_logic_vector(7 downto 0);

begin
	cnt_counter: process(CLK, RESET, cnt_inc, cnt_dec)
	begin
	 if RESET = '1' then
	  cnt_reg <= (others => '0');
	 elsif CLK'event and (CLK = '1') then
	  if cnt_inc = '1' then
	   cnt_reg <= cnt_reg + 1;
	  elsif cnt_dec = '1' then
	   cnt_reg <= cnt_reg - 1;
	  end if;
	 end if;
	end process; 

	pc_counter: process(CLK, RESET, pc_inc, pc_dec)
	begin
	 if RESET = '1' then
	  pc_reg <= (others => '0');
	 elsif CLK'event and (CLK = '1') then
	  if pc_inc = '1' then
	   if pc_reg = "0111111111111" then
	    pc_reg <= "0000000000000";
	   else
	    pc_reg <= pc_reg + 1;
	   end if;
	  elsif pc_dec = '1' then
	   if pc_reg = "0000000000000" then
	    pc_reg <= "0111111111111";
	   else
	    pc_reg <= pc_reg - 1;
	   end if;
	  end if;
	 end if;
	end process;
 
	ptr_counter: process(CLK, RESET, ptr_inc, ptr_dec)
	begin
	 if RESET = '1' then
	  ptr_reg <= "1000000000000";
	 elsif CLK'event and (CLK = '1') then
	  if ptr_inc = '1' then
	   if ptr_reg = "1111111111111" then
	    ptr_reg <= "1000000000000";
	   else
	    ptr_reg <= ptr_reg + 1;
	   end if;
	  elsif ptr_dec = '1' then
	   if ptr_reg = "1000000000000" then
	    ptr_reg <= "1111111111111";
	   else
	    ptr_reg <= ptr_reg - 1;
	   end if;
	  end if;
	 end if; 
	end process;

	mx1: process(CLK, RESET, mx1_sel)
	begin
	 if RESET = '1' then
	  DATA_ADDR <= (others => '0');
	 elsif CLK'event and (CLK = '1') then
	  if mx1_sel = '0' then
	   DATA_ADDR <= pc_reg;
	  elsif mx1_sel = '1' then
	   DATA_ADDR <= ptr_reg;
	  end if;
	 end if;
	end process; 
 

	mx2: process(CLK, RESET, mx2_sel)
	begin
	 if RESET = '1' then
	  DATA_WDATA <= (others => '0');
	 elsif CLK'event and (CLK = '1') then
	  case mx2_sel is
	   when "00" => DATA_WDATA <= IN_DATA;
	   when "01" => DATA_WDATA <= DATA_RDATA - 1;
	   when "10" => DATA_WDATA <= DATA_RDATA + 1;
	   when others => DATA_WDATA <= (others => '0');
	  end case;
	 end if;
	end process; 

	pstate_fsm: process(CLK, RESET, EN)	
	begin
	 if RESET = '1' then
	  pstate <= idle;

	 elsif CLK'event and (CLK = '1') then
	  if EN = '1' then
	   pstate <= nstate;
	  end if;
	 end if;
	end process;

	nstate_fsm: process(pstate, OUT_BUSY, IN_VLD, cnt_reg, DATA_RDATA)
	begin 
	 cnt_inc   <=  '0';
	 cnt_dec   <=  '0';
	 pc_inc    <=  '0';
	 pc_dec    <=  '0';
	 ptr_inc   <=  '0';
	 ptr_dec   <=  '0';
	 DATA_EN   <=  '0';
	 IN_REQ    <=  '0';
	 OUT_WE    <=  '0';
	 mx1_sel   <=  '0';
	 mx2_sel   <=  "00";
	 DATA_RDWR <=  '0';

	 case pstate is

	  when idle =>
		nstate <= fetch;

	  when fetch =>
		DATA_EN <= '1';
		nstate <= decode;

	  when decode =>
		case DATA_RDATA is 
		 when X"3E" => nstate <= state_ptr_inc;
		  ptr_inc <= '1';
		  pc_inc <= '1';
		 when X"3C" => nstate <= state_ptr_dec;
		  ptr_dec <= '1';
		  pc_inc <='1';
		 when X"2B" => nstate <= state_value_inc1;
		  DATA_EN <= '1';
		  mx1_sel <= '1';
		 when X"2D" => nstate <= state_value_dec1;
		  DATA_EN <= '1';
		  mx1_sel <= '1';
		 when X"5B" => nstate <= state_while_start1;
		  pc_inc <= '1';
		  DATA_EN <= '1';
		  mx1_sel <= '1';
		 when X"5D" => nstate <= state_while_end1;
		  DATA_EN <= '1';
		  mx1_sel <= '1';
		 when X"28" => nstate <= state_dowhile_start;
		 when X"29" => nstate <= state_dowhile_end1;
		  DATA_EN <= '1';
		  mx1_sel <= '1';
		 when X"2E" => nstate <= state_print1;
		  DATA_EN <= '1';
		  mx1_sel <= '1';
		 when X"2C" => nstate <= state_load1;
		  DATA_EN <= '1';
		  IN_REQ <= '1';
		  mx1_sel <= '1';
		 when X"00" => nstate <= state_null; 
		 when others => nstate <= actualize_addr;
		  pc_inc <= '1';
		end case;

	  when actualize_addr =>
		nstate <= fetch;

	  when state_ptr_inc => 
		DATA_EN <= '1';
		nstate <= fetch;

	  when state_ptr_dec =>
		DATA_EN <= '1';
		nstate <= fetch;

	  when state_value_inc1 =>
		DATA_EN <= '1';
		mx1_sel <= '1';
		nstate <= state_value_inc2;
	  when state_value_inc2 =>
		mx1_sel <= '1';
		mx2_sel <= "10";
		nstate <= state_value_inc3;
	  when state_value_inc3 =>
		mx1_sel <= '1';
		mx2_sel <= "10";
		nstate <= state_value_inc4;
	  when state_value_inc4 =>
		DATA_RDWR <= '1';
		DATA_EN <= '1';
		pc_inc <= '1';
		nstate <= actualize_addr; 
		
	  when state_value_dec1 => 
		DATA_EN <= '1';
		mx1_sel <= '1';
		nstate <= state_value_dec2;
	  when state_value_dec2 =>
		mx1_sel <= '1';
		mx2_sel <= "01";
		nstate <= state_value_dec3;
	  when state_value_dec3 =>
		mx1_sel <= '1';
		mx2_sel <= "01";
		nstate <= state_value_dec4;
	  when state_value_dec4 =>
		DATA_RDWR <= '1';
		DATA_EN <= '1';
		pc_inc <= '1';
		nstate <= actualize_addr;

	  when state_while_start1 =>
		DATA_EN <= '1';
		mx1_sel <= '1';
		nstate <= state_while_start2;
	  when state_while_start2 =>
		if DATA_RDATA = (DATA_RDATA'range => '0') then
		 DATA_EN <= '1';
		 nstate <= state_while_start3;
		else
		 nstate <= actualize_addr;
		end if;
	  when state_while_start3 =>
		DATA_EN <= '1';
		c <= DATA_RDATA;
		pc_inc <= '1';
	  when state_while_start4 =>
		if c /= X"5D" then
		 nstate <= state_while_start3;
		else
		 nstate <= actualize_addr;
		end if;

	  when state_while_end1 =>
		DATA_EN <= '1';
		nstate <= state_while_end2;
	  when state_while_end2 =>
		if DATA_RDATA /= (DATA_RDATA'range => '0') then
		 DATA_EN <= '1';
		 nstate <= state_while_end3;
		else
		 nstate <= state_while_end8;
		end if;
	  when state_while_end3 =>
		pc_dec <= '1';
		DATA_EN <= '1';
		nstate <= state_while_end4;
	  when state_while_end4 =>
		DATA_EN <= '1';
		nstate <= state_while_end5;
	  when state_while_end5 =>
		DATA_EN <= '1';
		nstate <= state_while_end6;
	  when state_while_end6 =>
		DATA_EN <= '1';
		c <= DATA_RDATA;
		nstate <= state_while_end7;
	  when state_while_end7 =>
		if c /= X"5B" then
		 nstate <= state_while_end3;
		else
		 nstate <= state_while_end8;
		end if;
	  when state_while_end8 =>
		pc_inc <= '1';
		nstate <= actualize_addr;

	  when state_dowhile_start => 
		pc_inc <= '1';
		nstate <= actualize_addr;

	  when state_dowhile_end1 => 
		DATA_EN <= '1';
		nstate <= state_dowhile_end2;
	  when state_dowhile_end2 =>
		if DATA_RDATA /= (DATA_RDATA'range => '0') then
		 DATA_EN <= '1';
		 nstate <= state_dowhile_end3;
		else
		 nstate <= state_dowhile_end8;
		end if;
	  when state_dowhile_end3 =>
		pc_dec <= '1';
		DATA_EN <= '1';
		nstate <= state_dowhile_end4;
	  when state_dowhile_end4 =>
		DATA_EN <= '1';
		nstate <= state_dowhile_end5;
	  when state_dowhile_end5 =>
		DATA_EN <= '1';
		nstate <= state_dowhile_end6;
	  when state_dowhile_end6 =>
		DATA_EN <= '1';
		c <= DATA_RDATA;
		nstate <= state_dowhile_end7;
	  when state_dowhile_end7 =>
		if c /= X"28" then
		 nstate <= state_dowhile_end3;
		else
		 nstate <= state_dowhile_end8;
		end if;
	  when state_dowhile_end8 =>
		pc_inc <= '1';
		nstate <= actualize_addr;

	  when state_print1 => 
		DATA_EN <= '1';
		mx1_sel <= '1';
		nstate <= state_print2;
	  when state_print2 =>
		if OUT_BUSY = '1' then
		 DATA_EN <= '1';
		 mx1_sel <= '1';
		 nstate <= state_print2;
		else
		 nstate <= state_print3;
		end if;
	  when state_print3 =>
		OUT_WE <= '1';
		OUT_DATA <= DATA_RDATA;
		pc_inc <= '1';
		nstate <= actualize_addr;

	  when state_load1 =>
		if IN_VLD = '0' then
		 IN_REQ <= '1';
		 mx1_sel <= '1';
		 DATA_EN <= '1';
		 nstate <= state_load1;
		else
		 mx1_sel <= '1';
		 DATA_EN <= '1';
		 nstate <= state_load2;
		end if;
	  when state_load2 =>
		DATA_RDWR <= '1';
		DATA_EN <= '1';
		pc_inc <= '1';
		nstate <= actualize_addr;

	  when state_null => 
		nstate <= state_null; 

	 end case; 
	end process;
 
end behavioral;

