(TeX-add-style-hook
 "refman"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "twoside" "12pt")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("tocloft" "titles") ("adjustbox" "export") ("inputenc" "utf8") ("wasysym" "nointegrals") ("xcolor" "table") ("hyperref" "pdftex" "pagebackref=true")))
   (add-to-list 'LaTeX-verbatim-environments-local "lstlisting")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperref")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperimage")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperbaseurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "nolinkurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "lstinline")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "lstinline")
   (TeX-run-style-hooks
    "latex2e"
    "classosse_1_1collaborate_1_1_attitude_matrix"
    "classosse_1_1collaborate_1_1_graph"
    "classosse_1_1collaborate_1_1_graph_unweighted"
    "classosse_1_1collaborate_1_1_graph_weighted"
    "classosse_1_1collaborate_1_1_simulation_clock"
    "classosse_1_1collaborate_1_1_tree"
    "structosse_1_1collaborate_1_1_tree_1_1_branch"
    "classosse_1_1collaborate_1_1_vector"
    "classosse_1_1collaborate_1_1_geodetic"
    "classosse_1_1collaborate_1_1_node"
    "structosse_1_1collaborate_1_1_node_1_1_partial_log"
    "classosse_1_1collaborate_1_1_observing_system"
    "classosse_1_1collaborate_1_1_observing_system_alpha"
    "classosse_1_1collaborate_1_1_orbital_state"
    "classosse_1_1collaborate_1_1_platform"
    "classosse_1_1collaborate_1_1_platform_earth"
    "classosse_1_1collaborate_1_1_platform_orbit"
    "classosse_1_1collaborate_1_1_reference_frame"
    "classosse_1_1collaborate_1_1_sun"
    "classosse_1_1collaborate_1_1_truth_data"
    "classosse_1_1collaborate_1_1_antenna"
    "classosse_1_1collaborate_1_1_antenna_dipole"
    "classosse_1_1collaborate_1_1_antenna_helical"
    "classosse_1_1collaborate_1_1_antenna_isotropic"
    "classosse_1_1collaborate_1_1_antenna_patch"
    "classosse_1_1collaborate_1_1_battery"
    "classosse_1_1collaborate_1_1_data_processor"
    "classosse_1_1collaborate_1_1_data_processor_sink"
    "classosse_1_1collaborate_1_1_data_processor_source"
    "classosse_1_1collaborate_1_1_modem"
    "classosse_1_1collaborate_1_1_modem_uhf_deploy"
    "classosse_1_1collaborate_1_1_modem_uhf_station"
    "classosse_1_1collaborate_1_1_power_subsystem"
    "classosse_1_1collaborate_1_1_rf_interface"
    "classosse_1_1collaborate_1_1_rf_interface_comm"
    "classosse_1_1collaborate_1_1_rf_interface_sensing"
    "classosse_1_1collaborate_1_1_sensor"
    "classosse_1_1collaborate_1_1_sensor_cloud_radar"
    "classosse_1_1collaborate_1_1_sensor_optical_imager"
    "classosse_1_1collaborate_1_1_sensor_rain_radar"
    "classosse_1_1collaborate_1_1_solar_panel"
    "classosse_1_1collaborate_1_1_channel"
    "classosse_1_1collaborate_1_1_packet"
    "classosse_1_1collaborate_1_1_packet_forward"
    "classosse_1_1collaborate_1_1_packet_raw"
    "classosse_1_1collaborate_1_1_packet_return"
    "classosse_1_1collaborate_1_1_scheduler"
    "classosse_1_1collaborate_1_1_scheduler_alpha"
    "article"
    "art12"
    "calc"
    "titleps"
    "kantlipsum"
    "doxygen"
    "graphicx"
    "makeidx"
    "multirow"
    "multicol"
    "textcomp"
    "amssymb"
    "sectsty"
    "geometry"
    "natbib"
    "amsmath"
    "ifpdf"
    "fancyhdr"
    "caption"
    "listings"
    "tocloft"
    "adjustbox"
    "inputenc"
    "wasysym"
    "xcolor"
    "hyperref")
   (TeX-add-symbols
    "project"
    "cpp"
    "cppeleven"
    "clearemptydoublepage")
   (LaTeX-add-labels
    "sec:start"
    "ssec:local"
    "sec:truth"
    "sec:osse"
    "ssec:build"
    "ssec:basic"
    "ssec:install"
    "sec:datastructures"
    "sec:physics"
    "sec:hardware"
    "sec:collaborative")
   (LaTeX-add-listings-lstdefinestyles
    "command"
    "cplusplus"
    "text")
   (LaTeX-add-xcolor-definecolors
    "myBlue"
    "myPurple"
    "myOrange"
    "myOcean"))
 :latex)

