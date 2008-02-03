$(header)

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
PACKAGE cond_chip_pkg IS

-- values for chip_id and version

CONSTANT chip_id: STD_LOGIC_VECTOR(31 DOWNTO 0) := X"00017132";
CONSTANT version: STD_LOGIC_VECTOR(31 DOWNTO 0) := X"$(version)";

-- algo of cond1_chip!!!

CONSTANT algo_chip_name: integer := 1;

-- algo-memory 6 x 1024 x 16 bit !!!!

CONSTANT mif_file : STRING := "algo_1024_16.mif";
CONSTANT mem_addr_cnt_width : integer := 10;
CONSTANT mem_data_width : integer := 16;
CONSTANT mem_width_base : integer := 3;
CONSTANT mem_width : integer := 6;

-- number of et-bits of calo

CONSTANT et_bits : integer := 6;

-- number of particle-conditions

$(conditions_nr)

END cond_chip_pkg;
