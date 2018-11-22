
    grid_rect_2D grid(reservoirTopLeftCornerInM, reservoirBottomRightCornerInM, ngrid);
    volField_rect_2D_cpu chi(grid);

    chi.fromFile(fileName);

    Sources_rect_2D src(sourcesTopLeftCornerInM, sourcesBottomRightCornerInM, nSourcesReceivers.src);
    
    src.Print();

    Receivers_rect_2D recv(src);
    recv.Print();

    Frequencies_group freqg(freq, c_0);
    freqg.Print(freq.nTotal);

    ProfileInterface *profiler;
    profiler = new ProfileCpu();
