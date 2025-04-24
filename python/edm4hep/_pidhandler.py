from ROOT import pythonization


@pythonization("PIDHandler", ns="edm4hep::utils")
def pythonize_pidhandler(klass):
    @classmethod
    def from_collections(cls, *collections):
        handler = cls()
        for collection in collections:
            handler.addColl(collection)
        return handler

    @classmethod
    def from_frame(cls, event, metadata=None):
        if metadata is None:
            return _from(event._frame)
        else:
            return _from(event._frame, metadata._frame)

    @classmethod
    def get_algo_info(cls, metadata, collname):
        return _get_algo_info(metadata._frame, collname)

    @classmethod
    def set_algo_info(cls, metadata, pidcoll, collname, pidMetaInfo):
        return _set_algo_info(metadata._frame, pidcoll, collname, pidMetaInfo)

    _from = getattr(klass, "from")
    klass.from_collections = from_collections
    klass.from_frame = from_frame

    _get_algo_info = klass.getAlgoInfo
    _set_algo_info = klass.setAlgoInfo
    klass.getAlgoInfo = get_algo_info
    klass.setAlgoInfo = set_algo_info
