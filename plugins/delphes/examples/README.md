# Testing Delphes -> EDM4hep


## Setting up tests

The following environment variables are expected to be set for the following to
work:
- `DELPHES_DIR`: points to the Delphes install directory
- `EDM4HEP_DIR`: points to the EDM4hep install directory
- `ROOT_INCLUDE_PATH`: contains the podio, EDM4hep and Delphes include
  directories (e.g. via `$EDM4HEP_DIR/include`)
- `LD_LIBRARY_PATH`: contains the podio, EDM4hep and Delphes libs (and dicts)
  (e.g. via `$EDM4HEP_DIR/lib`)


### Setting up a test folder

To easily run some tests, a folder is set up and some of the Delphes examples
are copied such that they can be easily changed without risking messing up the
Delphes installation somehow (however unlikely).

    mkdir test_delphes && cd test_delphes
    mkdir cards inputs
    cp ${DELPHES_DIR}/cards/delphes_card_{ILD,CMS}.tcl cards
    cp ${DELPHES_DIR}/examples/Pythia8/configNoLHE.cmnd inputs

To retrieve comparable outputs the random seeds for the Delphes card and the
Pythia8 configuration file are fixed. Additionally a restriction to 1000 event
is put in place. This means that

    set MaxEvents 1000
    set RandomSeed 123

is added at the top of `cards/delphes_card_CMS.tcl` (resp.
`cards/delphes_card_ILD.tcl`) and

    Random:setSeed = on
    Random:seed = 12345

is added in the first section (`1) Settings used in the main program`) of
`inputs/configNoLHE.cmnd`

Finally also copy the `edm4hep` output configuration to the `cards` directory
(not strictly necessary but in this case makes for cleaner commands in the
examples below)

    cp path/to/EDM4hep/plugins/delphes/edm4hep_output_config.tcl cards

## Generating Delphes output

A Delhpes output file is generated to have a baseline comparison for the edm4hep
output. The output is generated using the following command

    ${DELPHES_DIR}/bin/DelphesPythia8 \
        cards/delphes_card_CMS.tcl \
        inputs/configNoLHE.cmnd \
        delphes_test_output.root


## Generating edm4hep output

The corresponding edm4hep output is produced using the following command

    ${EDM4HEP_DIR}/bin/DelphesPythia8_EDM4HEP \
        cards/delphes_card_CMS.tcl \
        cards/edm4hep_output_config.tcl \
        inputs/configNoLHE.cmnd \
        edm4hep_test_output.root


## Generating comparison histograms

Two example macros that produce the same histograms once from a Delphes output
file and once from a edm4hep output file are in
`EDM4hep/plugins/delphes/examples`. Both take as input parameters the
corresponding filename and produce a new `.root` file with some histograms that
should ideally be equivalent (but are currently not).

    root -q 'path/to/EDM4hep/plugins/delphes/examples/read_delphes.C("delphes_test_output.root")'
    root -q 'path/to/EDM4hep/plugins/delphes/examples/read_edm4hep.C("edm4hep_test_output.root")' 

After this the files `histograms_delphes.root` and `histograms_edm4hep.root`
should be present and filled with some histograms that can be compared. The
differences in the Jet energy and momentum are caused by a potential double
counting of tracks and towers (see [known issues](../README.md#known-issues)).
