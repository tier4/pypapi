from ._papi import lib, ffi
from .types import Flips, Flops, IPC


# TODO (high api):
# [ ] accum_counters
# [ ] epc
# [x] flips
# [x] flops
# [x] ipc
# [ ] num_components
# [ ] num_counters
# [ ] read_counters
# [ ] start_counters
# [ ] stop_counters


def flips():
    """Simplified call to get Mflips/s (floating point instruction rate), real
    and processor time.
    """
    rtime = ffi.new("float*", 0)
    ptime = ffi.new("float*", 0)
    flpins = ffi.new("long long*", 0)
    mflips = ffi.new("float*", 0)

    rcode = lib.PAPI_flops(rtime, ptime, flpins, mflips)

    return Flips(
            rcode,
            ffi.unpack(rtime, 1)[0],
            ffi.unpack(ptime, 1)[0],
            ffi.unpack(flpins, 1)[0],
            ffi.unpack(mflips, 1)[0]
            )


def flops():
    """Simplified call to get Mflops/s (floating point operation rate), real
    and processor time.
    """
    rtime = ffi.new("float*", 0)
    ptime = ffi.new("float*", 0)
    flpops = ffi.new("long long*", 0)
    mflops = ffi.new("float*", 0)

    rcode = lib.PAPI_flops(rtime, ptime, flpops, mflops)

    return Flops(
            rcode,
            ffi.unpack(rtime, 1)[0],
            ffi.unpack(ptime, 1)[0],
            ffi.unpack(flpops, 1)[0],
            ffi.unpack(mflops, 1)[0]
            )


def ipc():
    """Gets instructions per cycle, real and processor time.
    """
    rtime = ffi.new("float*", 0)
    ptime = ffi.new("float*", 0)
    ins = ffi.new("long long*", 0)
    ipc_ = ffi.new("float*", 0)

    rcode = lib.PAPI_ipc(rtime, ptime, ins, ipc_)

    return IPC(
            rcode,
            ffi.unpack(rtime, 1)[0],
            ffi.unpack(ptime, 1)[0],
            ffi.unpack(ins, 1)[0],
            ffi.unpack(ipc_, 1)[0]
            )
