-- Test Vhdl file
entity native_test is
  port (
    clk   : in  bit;
    reset : in  bit;
    a     : in  bit;
    b     : in  bit;
    out_c : out bit;
    out_v : out bit_vector(7 downto 0)
  );
end entity;

-- ignore architecture for now

architecture behavior of native_test is
  signal c_internal : bit;
  signal bv1        : bit_vector(3 downto 0) := "1100";
  signal bv2        : bit_vector(3 downto 0) := "0011";
begin

  process(clk, reset)
    variable temp : integer := 0;
    variable valid : boolean := false;
    variable result : bit_vector(7 downto 0);
  begin
    if reset = '1' then
      c_internal <= '0';
      out_v <= (others => '0');

    elsif clk = '1' then
      -- native logic: bit ops, integer math, vector concat
      c_internal <= a and b;
      temp := 5 + 3;
      valid := not (temp < 10 or a /= b);
      result := bv1 & bv2;
      out_v <= result;
    end if;
  end process;

  out_c <= c_internal;

end architecture;