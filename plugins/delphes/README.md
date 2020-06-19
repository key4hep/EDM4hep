# Delphes to EDM4hep output converter

The `DelphesEDM4HepConverter` creates an `edm4hep` output file from a `delphes`
process.

## Configuration

The `DelphesEDM4HepConverter` takes an
[`OutputSetting`](DelphesEDM4HepConverter.h#L25-L98) parameter to configure which
`delphes` branches go into which `edm4hep` output collections. In the current
implementation of the conversions this is filled from an addition to the delphes
card that defines the necessary parameters:
[edm4hep_output_config.tcl](edm4hep_output_config.tcl). This `tcl` file has to
be placed in the **same directory as the delphes card** that is used and can then be
included in the main delphes `tcl` configuration using

``` tcl
source edm4hep_output_config.tcl
```

It is not possible to use absolute paths to include this file, as this will
crash the delphes `tcl` parser.
