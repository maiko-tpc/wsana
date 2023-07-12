Program to convert the GR daq file to ROOT file
====

## Description

## Requirement
[ROOT](https://root.cern.ch/) is required.

## Installation
Just type `$> make`

## Usage
- ./ana run****.bld run****.root [Options]

- Options
  - -online: Online mode. When the program reaches at the eond of file,
  it will stand-by and wait for another data.
  - -web [port num]: Enable THttp server. http://aino-1:[port num]
  - -last: Analyze from the last block.

- Examples
  - Offline analysis: `$> ./ana data/run7777.bld run7777.root`
  - Online analysis:  `$> ./ana data/run7777.bld out.root -online -last -web 5920`
  - Useful script for online monitor: `./online.sh`
    It analyzes the latest .bld data with -web 5920 and -last options.

## Tree branches

|Branch parameter|Data description|Note|
| ------- | ----------- | ------- |
|run|Run number||
|blk|Block number||
|eve|Event number|Block end events are skipped.|
|unixtime|Unix Time of the block||
|unixtimesub|Time in second from the run start||
|camac_inp[16]|CAMAC input register||
|vme_inpp[16]|VME input register by V1190 at vmepla||
|camac_sca[16]|CAMAC scaler||
|vme_sca[32]|VME scaler at vmepla||
|v1190_hit_ref[32]|V1190 reference hit flag|1:w/ hit, 0:w/o hit, [32] index is GEO address|
|grpla.fqdc[32]|GR FERA QDC||
|grpla.vqdc[32]|GR MQDC QDC||
|grpla.ftdc[32]|GR FERA TDC||
|grpla.vtdc[32]|vmepla V1190 TDC|[0--7]:GR PLA, [8--10]:RF, [11]:LAS trig, [14]:FCET trig, [15]: GR trig|
|grpla.pos[2]|GR PLA position by V1190 difference|[0]:PL1, [1]:PL2, Larger value means higher momentum|
|grpla.de[2]|GR PLA energy deposit by sqrt(QDC_L*QDC_R)|[0]:PL1, [1]:PL2|
|laspla.fqdc[32]|LAS FERA QDC||
|laspla.vqdc[32]|LAS MQDC QDC||
|laspla.ftdc[32]|LAS FERA TDC||
|laspla.vtdc[32]|vmepla V1190 TDC||
|laspla.pos[2]|LAS PLA position by V1190 difference|Larger value means higher momentum|
|laspla.de[2]|LAS PLA energy deposit by sqrt(QDC_L*QDC_R)||
|rf[3]|RF TDC|copy of grpla.vtdc[8--10]|
|vdcnhit[4]|Number of hits in GR VDC planes|[0]:X1, [1]:U1, [2]:X2, [3]:U2|
|meanwire[4]|Mean position of VDC hits in GR|[0]:X1, [1]:U1, [2]:X2, [3]:U2|
|nclst[4]|Number of clusters in GR VDC planes|[0]:X1, [1]:U1, [2]:X2, [3]:U2|
|clst_size[4]|Cluster size in GR VDC planes|[0]:X1, [1]:U1, [2]:X2, [3]:U2|
|gr_good_clst[4]|Good cluster flag for GR VDC planes|0:not good cluster, 1:good cluster|
|good_fit|GR VDC good fit flag|0:not good fit, 1:good fit|
|gr_tot_mean[4]|Mean values of time over threshold in GR VDC plane||
|gr_tot_max[4]|Max values of time over threshold in GR VDC plane||
|redchi2[4]|Reduced chi square of the fit in GR VDC||
|wire_pos[4]|Fitted position in GR VDC planes||
|residual[4]|Mean residual lengths in GR VDC planes|Unit in mm.|
|grx|GR X position|Unit in mm. Range: from -600 to 600. Larger values correspond to higher momentum.|
|gry|GR Y position|Unit in mm. Range: from -50 to 50. Larger values correspond to upper position.|
|grthx|GR X angle|Unit in degree. Larger values correspond to backward angle at scattering point.|
|grthy|GR Y angle|Unit in degree.|
|grp|GR momentum|Unit in MeV/c.|
|gre|GR energy|Unit in MeV.|
|vdcnhit_las[6]|Number of hits in LAS VDC planes|[0]:X1, [1]:U1, [2]:V1, [3]:X2, [4]:U2, [5]:V2|
|meanwire_las[6]|Mean position of VDC hits in LAS|[0]:X1, [1]:U1, [2]:V1, [3]:X2, [4]:U2, [5]:V2|
|las_tot_mean[6]|Mean values of time over threshold in LAS VDC plane||
|las_tot_max[6]|Max values of time over threshold in LAS VDC plane||

- Example
  - `tree->Draw("grthx:grx>>h1(200,-600,600, 200,-10,10)", "good_fit", "colz")`