library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
LIBRARY altera_mf;
USE altera_mf.altera_mf_components.all;

Entity testModLcell is port(
	clk, pi0, pi1, pi2, pi3: in std_logic;
	circ_out, circ_scan_data_out: out std_logic);
end testModLcell;

architecture structure of testModLcell is
	component MINV
	port (a: in std_logic; b: out std_logic);
	end component;

	component MNAND2
	port (a, b: in std_logic; c: out std_logic);
	end component;

	component MAND2
	port (a, b: in std_logic; c: out std_logic);
	end component;

	component MNOR2
	port (a, b: in std_logic; c: out std_logic);
	end component;

	component MOR2
	port (a, b: in std_logic; c: out std_logic);
	end component;

	component LCELL
	port (a_in : in std_logic; a_out : out std_logic);
	end component;

	component MDFF
	port (d, clk: in std_logic; q: out std_logic);
	end component;

	component MXOR2
	port (a, b: in std_logic; c: out std_logic);
	end component;

	signal s4, s5, s7, s9, s10, s11, s12, s13, s14, s15, s16, 
		lcOut_0, lcOut_1, lcOut_2, lcOut_3, lcOut_4, lcOut_5, lcOut_6, mod_s4, mod_s5, mod_s7, 
		mod_s9, mod_s10, mod_s11, mod_s12, mod_s13, mod_s14, mod_s15, mod_s16, scan_data_out, mod_scan_data_out, 
		tmpLcOut_0, po0, mod_po0: std_logic;

begin
-- Original circuit
	g14: minv port map (pi0, s7);
	g12: mnor2 port map (pi1, scan_data_out, s15);
	g8: mand2 port map (s7, s5, s9);
	g13: mnor2 port map (pi2, s15, s16);
	g16: mor2 port map (pi3, s9, s11);
	g15: mor2 port map (s15, s9, s10);
	g9: mnand2 port map (s11, s10, s12);
	g11: mnor2 port map (s4, s12, s14);
	g10: mnor2 port map (s7, s14, s13);
	g17: minv port map (s14, po0);
	ff_0: mdff port map (s16, clk, scan_data_out);
	ff_1: mdff port map (s14, clk, s5);
	ff_2: mdff port map (s13, clk, s4);

-- Circuit with LCELLs
	modCirc_g14: minv port map (pi0, mod_s7);
	modCirc_g12: mnor2 port map (pi1, mod_scan_data_out, mod_s15);
	modCirc_g8: mand2 port map (lcOut_4, mod_s5, mod_s9);
	modCirc_g13: mnor2 port map (pi2, lcOut_6, mod_s16);
	modCirc_g16: mor2 port map (pi3, lcOut_3, mod_s11);
	modCirc_g15: mor2 port map (lcOut_6, lcOut_3, mod_s10);
	modCirc_g9: mnand2 port map (mod_s11, mod_s10, mod_s12);
	modCirc_g11: mnor2 port map (mod_s4, mod_s12, mod_s14);
	modCirc_g10: mnor2 port map (lcOut_4, lcOut_2, mod_s13);
	modCirc_g17: minv port map (lcOut_2, mod_po0);
	modCirc_lc_0: lcell port map (mod_s13, lcOut_0);
	modCirc_lc_1: lcell port map (mod_po0, lcOut_1);
	modCirc_lc_2: lcell port map (mod_s14, lcOut_2);
	modCirc_lc_3: lcell port map (mod_s9, lcOut_3);
	modCirc_lc_4: lcell port map (mod_s7, lcOut_4);
	modCirc_lc_5: lcell port map (mod_s16, lcOut_5);
	modCirc_lc_6: lcell port map (mod_s15, lcOut_6);
	modCirc_ff_0: mdff port map (lcOut_5, clk, mod_scan_data_out);
	modCirc_ff_1: mdff port map (tmpLcOut_0, clk, mod_s5);
	modCirc_ff_2: mdff port map (lcOut_0, clk, mod_s4);
	modCirc_tmpLc_0: lcell port map (lcOut_2, tmpLcOut_0);

	g00: mxor2 port map (po0, lcOut_1, circ_out);
	g01: mxor2 port map (scan_data_out, mod_scan_data_out, circ_scan_data_out);

end structure;

-- Behavioural description of all components
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity MINV is port(
	a: in std_logic;
	b: out std_logic);
end MINV;

architecture behaviour of MINV is
begin
	b <= not a;
end behaviour;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity MNAND2 is port(
	a, b: in std_logic;
	c: out std_logic);
end MNAND2;

architecture behaviour of MNAND2 is
begin
	c <= not(a and b);
end behaviour;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity MAND2 is port(
a, b: in std_logic;
c: out std_logic);
end MAND2;

architecture behaviour of MAND2 is
begin
	c <= a and b;
end behaviour;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity MNOR2 is port(
	a, b: in std_logic;
	c: out std_logic);
end MNOR2;

architecture behaviour of MNOR2 is

begin
	c <= not(a or b);
end behaviour;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity MOR2 is port(
	a, b: in std_logic;
	c: out std_logic);
end MOR2;

architecture behaviour of MOR2 is
begin
	c <= a or b;
end behaviour;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity MLCELL is port(
	Input_A: in std_logic;
	Input_B: in std_logic;
	Output_Z: out std_logic);
end MLCELL;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity MDFF is port(
	d, clk: in std_logic;
	q: out std_logic);
end MDFF;

architecture behaviour of MDFF is
begin
	process (clk)
	begin
		if (clk'event and clk = '1') then
		q <= d;
		end if;
	end process;
end behaviour;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity MXOR2 is port(
	a, b: in std_logic;
	c: out std_logic);
end MXOR2;

architecture behaviour of MXOR2 is
begin
	c <= a xor b;
end behaviour;

