
This version of tlm works with SystemC 2.1. It no longer provides an
implementation of sc_export to work with SystemC 2.0.1.

Changes since the last version
- reorg of directories
- split tlm_req_rsp_channel and tlm_transport_channel
--- master and slave exported but not implemented
- added peek and get_peek interfaces
- deleted shrink/unshrink, pop, moded and extended interfaces
- SCOPING_BUG deleted
- deleted sc_export.h






