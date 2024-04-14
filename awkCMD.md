./waf --run scratch/second

awk -f throughput.awk tcp-bulk-send.tr
awk -f throughput-graph.awk tcp-bulk-send.tr > graph-Throughput 
gnuplot Throughput-graphcode

awk -f delay.awk l3q1.tr
awk -f delay-graph.awk tcp-bulk-send.tr > graph-delay
gnuplot delay-graphcode

awk -f pdf.awk l3q1.tr
awk -f pdf-graph.awk tcp-bulk-send.tr > graph-pdf
gnuplot pdf-graphcode