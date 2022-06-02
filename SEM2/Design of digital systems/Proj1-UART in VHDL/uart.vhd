-- uart.vhd: UART controller - receiving part
-- Author(s): xondre15
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

-------------------------------------------------
entity UART_RX is
port(	
  CLK: 	     in  std_logic;
	RST: 	     in  std_logic;
	DIN: 	     in  std_logic;
	DOUT: 	    out std_logic_vector(7 downto 0);
	DOUT_VLD: 	out std_logic
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is
  signal bit_read   : std_logic;
  signal clk_cnt_en : std_logic;
  signal out_vld    : std_logic;
  signal clk_cnt    : std_logic_vector(4 downto 0);
  signal bit_cnt    : std_logic_vector(3 downto 0);
begin
  FSM: entity work.UART_FSM(behavioral)
  port map(
    CLK      => CLK,
    RST      => RST,
    DIN      => DIN,
    MSB_READ => bit_cnt(3),
    CLK_CNT  => clk_cnt,
    CLK_INC  => clk_cnt_en,
    DATA_INC => bit_read,
    VALID    => out_vld
  );
  
  DOUT_VLD <= out_vld;
  process(CLK) begin
    if rising_edge(CLK) then
      if RST = '1' then
        clk_cnt <= "00000";
        bit_cnt <= "0000";
      else
        if clk_cnt_en = '1' then
          clk_cnt <= clk_cnt+1;
        else
          clk_cnt <= "00000";
        end if;
        if bit_read = '1' and clk_cnt(4) = '1' then
          DOUT(conv_integer(bit_cnt)) <= DIN;
          bit_cnt <= bit_cnt+1;
          clk_cnt <= "00001";
        end if;
        if bit_read = '0' then
          bit_cnt <= "0000";
        end if;
      end if;
    end if;
  end process;
end behavioral;
