! main03.cmnd.
! This file contains commands to be read in for a Pythia8 run.
! Lines not beginning with a letter or digit are comments.
! Names are case-insensitive  -  but spellings-sensitive!
! The settings here are illustrative, not always physics-motivated.

! 1) Settings used in the main program.
Random:setSeed = on
Random:seed = 10
Main:numberOfEvents = 1000         ! number of events to generate
Main:timesAllowErrors = 5          ! how many aborts before run stops

! 2) Settings related to output in init(), next() and stat().
Init:showChangedSettings = on      ! list changed settings
Init:showChangedParticleData = off ! list changed particle data
Next:numberCount = 100             ! print message every n events
Next:numberShowInfo = 1            ! print event information n times
Next:numberShowProcess = 1         ! print process record n times
Next:numberShowEvent = 0           ! print event record n times

! 3) Beam parameter settings. Values below agree with default ones.
Beams:idA = 11                   ! first beam, e = 2212, pbar = -2212
Beams:idB = -11                   ! second beam, e = 2212, pbar = -2212

! 4) Hard process : Z->qqbar at Ecm=91 GeV
Beams:eCM = 91  ! CM energy of collision

!WeakSingleBoson:ffbar2ffbar(s:gmZ) = on 
!WeakZ0:gmZmode = 2
!23:onMode = off
!23:onIfMatch = 5 5

WeakSingleBoson:ffbar2gmZ = on
23:onMode = off
23:onIfAny = 5
