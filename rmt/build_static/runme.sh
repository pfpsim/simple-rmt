rm *.csv
if [ "$#" -ne 3 ]; then
    echo "Invalid Args passed"
    echo "Usage: $0 <PFP debugger: [none,debug]> <application: [pfp, nat, router, ecmp, firewall]> <verbosity level: [normal, minimal, p4profile, profile, debugger, debug]>"
    exit
fi
debug_level="$1"
application="$2"
vlvl="$3"

if [[ $debug_level == "debug" ]] ; then
    if [[ $application == "pfp" ]] ; then 
        pfpdb rmt-sim --args "-Xp4 ../../../apps/pfp_baseline/simple_router.json -Xtpop ../../../apps/pfp_baseline/table.txt -v "$vlvl" -Xin ../../../apps/pfp_baseline/input.pcap -Xvalidation-out output.pcap" -v
    else
        echo ""$application" not yet implemented.."
        exit
    fi
else
    if [[ $application == "pfp" ]] ; then
        ./rmt-sim -c Configs/ -Xp4 ../../../apps/pfp_baseline/simple_router.json -Xtpop ../../../apps/pfp_baseline/table.txt -Xin ../../../apps/pfp_baseline/input.pcap -Xvalidation-out reordered-output.pcap -v "$vlvl"
    else 
        echo ""$application" not yet implemented.."
        exit
    fi
fi
