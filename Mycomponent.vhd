library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
LIBRARY altera_mf;
USE altera_mf.altera_mf_components.all;


entity Mycomponent is port( 	
	PI0, PI1, PI2, PI3, clk, set, reset: in bit;	
	PO0: out bit;
	scan_data_out:buffer bit);
end Mycomponent;

			
Architecture structure of Mycomponent is
	component MINV
	port (a: in bit; b: out bit); 
	end component;
	
	component  MAND2
	port (	a, b: in bit; c: out bit); 
	end component;
	
	component MNAND2
	port (a, b: in bit; c: out bit); 
	end component;
	
	component MOR2
	port (a, b: in bit; c: out bit);
	end component;
	
	component MNOR2
	port (a, b: in bit; c: out bit);
	end component;
	
	component MDFF
	port (d, clk, set, reset: in bit; q: out bit);
	end component;
	
	signal S4, S5, S7, S9, S10, S11, S12, S13, S14, S15, S16: bit;
	
begin
	G14: MINV port map (PI0, S7);
	G12: MNOR2 port map (PI1, scan_data_out, S15);
	G8: MAND2 port map (S7, S5, S9);
	G13: MNOR2 port map (PI2, S15, S16);
	G16: MOR2 port map (PI3, S9, S11);	
	G15: MOR2 port map (S15, S9, S10);	
	G7: MDFF port map (S16, clk, set, reset, scan_data_out);	
	G9: MNAND2 port map (S11, S10, S12);
	G11: MNOR2 port map (S4, S12, S14);
	G10: MNOR2 port map (S7,S14, S13);
	G17: MINV port map (S14, PO0);
	G6: MDFF port map (S14, clk, set, reset, S5);
	G5: MDFF port map (S13, clk, set, reset, S4);
end structure;

-- Behavioural description of all components

entity MINV is port(
	a: in bit; 
	b: out bit); 
end MINV;

architecture behaviour of MINV is
begin
	b <= not a;
 end behaviour;
 

 entity MAND2 is port(
	a, b: in bit;
	c: out bit);
end MAND2;

architecture behaviour of MAND2 is
begin
	c <= a and b;
end behaviour;


entity MNAND2 is port(
	a, b: in bit;
	c: out bit);
end MNAND2;

architecture behaviour of MNAND2 is
begin
	c <= not(a and b);
end behaviour;


entity MOR2 is port(
	a, b: in bit;
	c: out bit);
end MOR2;

architecture behaviour of MOR2 is
begin
	c <= a or b;
end behaviour;


entity MNOR2 is port(
	a, b: in bit;
	c: out bit);
end MNOR2;
	
architecture behaviour of MNOR2 is
begin
	c <= not(a or b);
end behaviour;


entity MDFF is port(
	d, clk, set, reset: in bit; 
	q: out bit);
end MDFF;

architecture behaviour of MDFF is
begin
	process (clk, set, reset)
	begin
		if reset = '1' then
		q <= '0';
		elsif set = '1' then
		q <= '1';
		elsif (clk'event and clk = '1') then
		q <= d;
		end if;
	end process;
end behaviour;