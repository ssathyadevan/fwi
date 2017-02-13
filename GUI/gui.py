import Tkinter
import os
from numpy import *

def run():
    str_run = "cd ../build-try_qt-Desktop-Release/ && xterm -hold -e mpirun -np " + nprocs.get() + " ./try_qt"
    print str_run
    os.system(str_run)

def Write_input_file():
    file1 = open("../include/input_parameters.h" , "w")
    file1.write("#ifndef INPUT_PARAMETERS\n")
    file1.write("#define INPUT_PARAMETERS\n")
    file1.write("const int nxt = " + nxt.get() + ";\n")
    file1.write("const int nzt = " + nzt.get() + ";\n")
    file1.write("const int nSrct = " + nSrct.get() + ";\n")
    file1.write("const int nFreq_Total = " + nFreq_Total.get() + ";\n")
    file1.write("const int calc_alpha = " + calc_alpha.get() + ";\n")
    file1.write("const int n_max = " + n_max.get() + ";\n")
    file1.write("const int n_iter1 = " + n_iter1.get() + ";\n")
    file1.write("const int n_iter2 = " + n_iter2.get() + ";\n")
    file1.write("const double tol1 = " + tol1.get() + ";\n")
    file1.write("const double tol2 = " + tol2.get() + ";\n")
    file1.write("const int do_reg = " + do_reg.get() + ";\n")
    file1.write("const int interactive = " + interactive.get() + ";\n")
    file1.write("const double F_min1 = " + F_min1.get() + ";\n")
    file1.write("const double F_max1 = " + F_max1.get() + ";\n")
    file1.write("const int freq_dist_group = " + Freq_dist.get() + ";\n")
    file1.write("const int nFreq_input[] = {" + nFreq_input.get() + "};\n")
    file1.write("const int gpu = " + gpu.get() + ";\n")
    file1.write("const double delta_amplification_start = " + delta_start.get() + ";\n")
    file1.write("const double delta_amplification_slope = " + delta_slope.get() + ";\n")
    file1.write("#endif // INPUT_PARAMETERS")
    file1.close()

def Recompile():
    os.system("cd ../build-try_qt-Desktop-Release/ && xterm -e make clean")
    os.system("cd ../build-try_qt-Desktop-Release/ && xterm -e make -j8")

def Compile():
    os.system("cd ../build-try_qt-Desktop-Release/ && xterm -e make -j8")

main = Tkinter.Tk()

n_max = Tkinter.StringVar()
n_iter1 = Tkinter.StringVar()
n_iter2 = Tkinter.StringVar()
tol1 = Tkinter.StringVar()
tol2 = Tkinter.StringVar()
do_reg = Tkinter.StringVar()
calc_alpha = Tkinter.StringVar()
nxt = Tkinter.StringVar()
nzt = Tkinter.StringVar()
nSrct = Tkinter.StringVar()
nFreq_Total = Tkinter.StringVar()
interactive = Tkinter.StringVar()
F_min1 = Tkinter.StringVar()
F_max1 = Tkinter.StringVar()
Freq_dist = Tkinter.StringVar()
nFreq_input = Tkinter.StringVar()
gpu = Tkinter.StringVar()
delta_start = Tkinter.StringVar()
delta_slope = Tkinter.StringVar()
nprocs = Tkinter.StringVar()

frame = Tkinter.Frame(main, bg="white", width=825, height=625)
frame.pack()
#frame.grid(row=20, column=250)
frame.pack_propagate(0)
frame.grid_propagate(0)


nxt.set(128)
nzt.set(64)
nSrct.set(17)
nFreq_Total.set(20)
calc_alpha.set(0)
n_max.set(5)
n_iter1.set(50)
n_iter2.set(100)
tol1.set(1e-5)
tol2.set(5e-5)
do_reg.set(1)
interactive.set(0)
F_min1.set(10.0)
F_max1.set(40.0)
Freq_dist.set(1)
nFreq_input.set(1)
gpu.set(1)
delta_start.set(100.0)
delta_slope.set(10.0)
nprocs.set(1)


label0 = Tkinter.Label(frame, text="press run to run the simulation")
#label0.pack(side=Tkinter.TOP)
label0.grid(row=0, column=2)

label1 = Tkinter.Label(frame, text="max_outer iterations")
label1.grid(row=1, column=0, padx=4, pady=4)

entry1 = Tkinter.Entry(frame, width=5, textvariable=n_max)
entry1.grid(row=1, column=1, padx=4, pady=4)



label2 = Tkinter.Label(frame, text="max CG iterations")
label2.grid(row=2, column=0, padx=4, pady=4)

entry2 = Tkinter.Entry(frame, width=5, textvariable=n_iter1)
entry2.grid(row=2, column=1, padx=4, pady=4)

label21 = Tkinter.Label(frame, text="tol CG iterations")
label21.grid(row=2, column=2, padx=4, pady=4)

entry21 = Tkinter.Entry(frame, width=5, textvariable=tol1)
entry21.grid(row=2, column=3, padx=4, pady=4)


label3 = Tkinter.Label(frame, text="max p_tot iterations")
label3.grid(row=3, column=0, padx=4, pady=4)

entry3 = Tkinter.Entry(frame, width=5, textvariable=n_iter2)
entry3.grid(row=3, column=1, padx=4, pady=4)

label31 = Tkinter.Label(frame, text="tol p_tot iterations")
label31.grid(row=3, column=2, padx=4, pady=4)

entry31 = Tkinter.Entry(frame, width=5, textvariable=tol2)
entry31.grid(row=3, column=3, padx=4, pady=4)


label4 = Tkinter.Label(frame, text="Calc_alpha")
label4.grid(row=4, column=0, padx=4, pady=4)

entry4 = Tkinter.Entry(frame, width=5, textvariable=calc_alpha)
entry4.grid(row=4, column=1, padx=4, pady=4)

label4 = Tkinter.Label(frame, text="\\\put 1 to calculate alpha, 0 otherwise")
label4.grid(row=4, column=2, padx=4, pady=4)


label5 = Tkinter.Label(frame, text="Do regularization")
label5.grid(row=5, column=0, padx=4, pady=4)

entry5 = Tkinter.Entry(frame, width=5, textvariable=do_reg)
entry5.grid(row=5, column=1, padx=4, pady=4)

label5 = Tkinter.Label(frame, text="\\\put 1 to turn on regularization, 0 otherwise")
label5.grid(row=5, column=2, padx=4, pady=4)


label6 = Tkinter.Label(frame, text="no. ofcells x")
label6.grid(row=6, column=0, padx=4, pady=4)

entry6 = Tkinter.Entry(frame, width=5, textvariable=nxt)
entry6.grid(row=6, column=1, padx=4, pady=4)


label61 = Tkinter.Label(frame, text="no. of cells y")
label61.grid(row=6, column=2, padx=4, pady=4)

entry61 = Tkinter.Entry(frame, width=5, textvariable=nzt)
entry61.grid(row=6, column=3, padx=4, pady=4)


label7 = Tkinter.Label(frame, text="no. of Sources and Receivers")
label7.grid(row=7, column=0, padx=4, pady=4)

entry7 = Tkinter.Entry(frame, width=5, textvariable=nSrct)
entry7.grid(row=7, column=1, padx=4, pady=4)


label8 = Tkinter.Label(frame, text="min Freq.")
label8.grid(row=8, column=0, padx=4, pady=4)

entry8 = Tkinter.Entry(frame, width=5, textvariable=F_min1)
entry8.grid(row=8, column=1, padx=4, pady=4)


label81 = Tkinter.Label(frame, text="max Freq.")
label81.grid(row=8, column=2, padx=4, pady=4)

entry81 = Tkinter.Entry(frame, width=5, textvariable=F_max1)
entry81.grid(row=8, column=3, padx=4, pady=4)


label9 = Tkinter.Label(frame, text="no. of  Freq.")
label9.grid(row=9, column=0, padx=4, pady=4)

entry9 = Tkinter.Entry(frame, width=5, textvariable=nFreq_Total)
entry9.grid(row=9, column=1, padx=4, pady=4)


label91 = Tkinter.Label(frame, text="freq_dist (1 for group distribution of freq. else 0)")
label91.grid(row=9, column=2, padx=4, pady=4)

entry91 = Tkinter.Entry(frame, width=5, textvariable=Freq_dist)
entry91.grid(row=9, column=3, padx=4, pady=4)


label10 = Tkinter.Label(frame, text="num. of Freq for each proc")
label10.grid(row=10, column=0, padx=4, pady=4)

entry101 = Tkinter.Entry(frame, width=5, textvariable=nFreq_input)
entry101.grid(row=10, column=1, padx=4, pady=4)

label102 = Tkinter.Label(frame, text="\\\just enter 1 for even distribution amongst procs")
label102.grid(row=10, column=2, padx=4, pady=4)


label11 = Tkinter.Label(frame, text="gpu")
label11.grid(row=11, column=0, padx=4, pady=4)

entry111 = Tkinter.Entry(frame, width=5, textvariable=gpu)
entry111.grid(row=11, column=1, padx=4, pady=4)

label112 = Tkinter.Label(frame, text="\\\enter 1 to turn on gpu computations, else 0")
label112.grid(row=11, column=2, padx=4, pady=4)


label12 = Tkinter.Label(frame, text="delta for regularization")
label12.grid(row=12, column=0, padx=4, pady=4)

entry12 = Tkinter.Entry(frame, width=5, textvariable=delta_start)
entry12.grid(row=12, column=1, padx=4, pady=4)


label121 = Tkinter.Label(frame, text="slope for decrementing delta")
label121.grid(row=12, column=2, padx=4, pady=4)

entry121 = Tkinter.Entry(frame, width=5, textvariable=delta_slope)
entry121.grid(row=12, column=3, padx=4, pady=4)


label13 = Tkinter.Label(frame, text="interactive")
label13.grid(row=13, column=0, padx=4, pady=4)

entry13 = Tkinter.Entry(frame, width=5, textvariable=interactive)
entry13.grid(row=13, column=1, padx=4, pady=4)

label113 = Tkinter.Label(frame, text="\\\enter 1 for interactive python session, else 0")
label113.grid(row=13, column=2, padx=4, pady=4)


label14 = Tkinter.Label(frame, text="no. of procs")
label14.grid(row=14, column=0, padx=4, pady=4)

entry14 = Tkinter.Entry(frame, width=5, textvariable=nprocs)
entry14.grid(row=14, column=1, padx=4, pady=4)


button1 = Tkinter.Button(frame, text="run", command=run)
button1.grid(row=20, column=2, padx=4, pady=4)


button2 = Tkinter.Button(frame, text="Write_input_file", command=Write_input_file)
button2.grid(row=18, column=1, padx=4, pady=4)


button3 = Tkinter.Button(frame, text="Build", command=Compile)
button3.grid(row=18, column=2, padx=4, pady=4)


button4 = Tkinter.Button(frame, text="Re-build", command=Recompile)
button4.grid(row=18, column=3, padx=4, pady=4)

main.mainloop()

