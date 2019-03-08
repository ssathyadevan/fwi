cp implementations_comparison_python3.py ~/FWIInstall
cd ~/FWIInstall
rm -rf fast/ fastNEW/
cp -r ~/parallelized-fwi/tests/regression_data/fast/ .
python3 implementations_comparison_python3.py bin/FWI_PreProcess bin/FWI_Process fast
cd - 1>/dev/null
