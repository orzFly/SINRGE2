# -*- ruby -*-
# for backward compatibility
warn "Warning:#{caller[0].sub(/:in `.*'\z/, '')}: Win32API is deprecated after Ruby 1.9.1; use dl directly instead" if $VERBOSE

require 'dl.so'

class Win32API
  DLL = {}
  TYPEMAP = {"0" => DL::TYPE_VOID, "S" => DL::TYPE_VOIDP, "I" => DL::TYPE_LONG}
  POINTER_TYPE = DL::SIZEOF_VOIDP == DL::SIZEOF_LONG_LONG ? 'q*' : 'l!*'

  def initialize(dllname, func, import, export = "0", calltype = :stdcall)
    @proto = [import].join.tr("VPpNnLlIi", "0SSI").sub(/^(.)0*$/, '\1')
    handle = DLL[dllname] ||= DL.dlopen(dllname)
    @func = DL::CFunc.new(handle[func], TYPEMAP[export.tr("VPpNnLlIi", "0SSI")], func, calltype)
  rescue DL::DLError => e
    raise LoadError, e.message, e.backtrace
  end

  def call(*args)
    import = @proto.split("")
    args.each_with_index do |x, i|
      args[i], = [x == 0 ? nil : x].pack("p").unpack(POINTER_TYPE) if import[i] == "S"
      args[i], = [x].pack("I").unpack("i") if import[i] == "I"
    end
    ret, = @func.call(args)
    return ret || 0
  end

  alias Call call
end