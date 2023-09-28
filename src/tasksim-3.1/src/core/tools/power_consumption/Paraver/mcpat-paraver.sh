#! /bin/sh 

echo "Generating McPat input"
~/code/tasksim-mcpat.py --out=mcpat-input.xml test.config results.txt 1 0
echo "Executing McPAT..."
~/bin/McPAT/mcpat -infile mcpat-input.xml -print_level 4 > power_stats.txt

L1write=$(grep L1Write power_stats.txt | awk '{printf ("%.13f", $2); exit}') #trick to sum cientific numbers
L1avg=$(grep L1Avg power_stats.txt | awk '{printf ("%.13f", $2); exit}') #trick to sum cientific numbers
L2write=$(grep L2Write power_stats.txt | awk '{printf ("%.13f", $2); exit}') #trick to sum cientific numbers
L2avg=$(grep L2Avg power_stats.txt | awk '{printf ("%.13f", $2); exit}') #trick to sum cientific numbers

L1avgT=$(echo "scale=6;$L1avg*1000"| bc -q | sed 's/^\./0./;s/0*$//;s/\.$//')    #op floats
L2avgT=$(echo "scale=6;$L2avg*1000"| bc -q | sed 's/^\./0./;s/0*$//;s/\.$//')    #op floats
L1writeT=$(echo "scale=6;$L1write*1000"| bc -q | sed 's/^\./0./;s/0*$//;s/\.$//')    #op floats
L2writeT=$(echo "scale=6;$L2write*1000"| bc -q | sed 's/^\./0./;s/0*$//;s/\.$//')    #op floats

sed -i "154s/.*window_factors.*/window_factors $L1avgT $L1writeT/" cfgs/Total.cfg
sed -i "333s/.*window_factors.*/window_factors $L2avgT $L2writeT/" cfgs/Total.cfg
