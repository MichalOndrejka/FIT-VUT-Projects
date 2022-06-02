-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): xondre15
--
library ieee;
use ieee.std_logic_1164.all;

-------------------------------------------------
entity UART_FSM is
port(
   CLK         : in std_logic;
   RST         : in std_logic;
   DIN         : in std_logic;
   MSB_READ    : in std_logic;
   CLK_CNT     : in std_logic_vector(4 downto 0);
   CLK_INC     : out std_logic;
   DATA_INC    : out std_logic;
   VALID       : out std_logic
   );
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is
type STATE_TYPE is (IDLE, START, LOAD, WAIT_END, VALIDATE);
signal state : STATE_TYPE := IDLE;
begin
  CLK_INC  <= '0' when state = IDLE else '1';
  DATA_INC <= '1' when state = LOAD else '0';
  VALID    <= '1' when state = VALIDATE else '0';
  process (CLK) begin
    if rising_edge(CLK) then
      if RST = '1' then
        state <= IDLE;
      else
        case state is
        when IDLE =>
          if DIN = '0' then
            state <= START;
          end if;
        when START =>
          if CLK_CNT = "10110" then
            state <= LOAD;
          end if;
        when LOAD =>
          if MSB_READ = '1' then
            state <= WAIT_END;
          end if;
        when WAIT_END =>
          if CLK_CNT = "10000" then
            state <= VALIDATE;
          end if;
        when VALIDATE =>
          state <= IDLE;
        
        end case;
      end if;
    end if;
  end process;
end behavioral;
