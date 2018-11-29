// this chunk of code is repeated in main_preProcessing and main_processing,
// to create objects needed for Forward Model and Inversion, such as grid,
// sources, receivers etc. Hence we put it here in a seperate header file

grid_rect_2D grid(input.reservoirTopLeftCornerInM, input.reservoirBottomRightCornerInM, input.ngrid);
volField_rect_2D_cpu chi(grid);

chi.fromFile(input.fileName);

Sources_rect_2D src(input.sourcesTopLeftCornerInM, input.sourcesBottomRightCornerInM, input.nSourcesReceivers.src);

src.Print();

Receivers_rect_2D recv(src);
recv.Print();

Frequencies_group freqg(input.freq, input.c_0);
freqg.Print(input.freq.nTotal);

ProfileInterface *profiler;
profiler = new ProfileCpu();
