-- simann.ads	Specification for SimAnneal
--   a collection of routines for general purpose Simulated Annealing
--   intended to be the Ada equivalent of the C++ Simulated Annealing object
--   SimAnneal
--                      (c) Copyright 1994, Everett F. Carter Jr.
--                      Permission is granted by the author to use
--			this software for any application provided this
--			copyright notice is preserved.

--   Uses Cauchy training

with BArray; use BArray;

generic
with function func(x : in BasicArrayHandle) return float;

package SimAnneal is

	function SAInit(d : integer ) return integer;
        procedure SAFree;
        function SAIterations(m : integer := -1) return integer;
        function SABoltzmann( k : float := -1.0) return float;
        function SATemperature(t : float := -1.0) return float;
        function SALearning_Rate(r : float := -1.0) return float;
        function SAJump(j : float := -1.0) return float;
        procedure SAInitial(xi : in  BasicArrayHandle);
        procedure SACurrent(xc : BasicArrayHandle);
        procedure SAOptimum(xb : BasicArrayHandle);
        function SAMelt(iters : integer := -1) return float;
        function SAAnneal(iters : integer := -1) return float;
        
end SimAnneal;

