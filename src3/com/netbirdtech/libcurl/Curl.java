class Curl{
   public Curl() {
		if (!INIT) {
			curlGlobalInit(CurlConstant.CURL_GLOBAL_DEFAULT);
		}
	}

	public static void curlGlobalInit(int flags) {
		if (INIT) {
			return;
		}

		CurlCode code = CurlCode.fromValue(curlGlobalInitNative(flags));
		if (code != CurlCode.CURLE_OK) {
			throw new IllegalStateException("curlGlobalInit fail: " + code);
		}
		INIT = true;
	}

	private native static int curlGlobalInitNative(int flags);

	private native static void curlGlobalCleanupNative();

	public void curlEasyInit() throws CurlException {
		// Log.v(TAG, "curlEastInit");
		handle = curlEasyInitNative();
		if (handle == 0) {
			throw new CurlException("curl init native fail");
		}
	}

	private native long curlEasyInitNative();

	public void curlEasyCleanup() {
		// Log.v(TAG, "curlEastCleanup: " + handle);
		if (handle != 0) {
			curlEasyCleanupNative(handle);
		}
		handle = 0;
	}

	private native void curlEasyCleanupNative(long handle);
   /**
	 * 
	 * @param opt
	 *            {@link OptLong}
	 * @param value
	 * @return
	 */
	public CurlCode curlEasySetopt(OptLong opt, long value) {
		// Log.v(TAG, "curlEastSetopt: " + opt + "=" + value);
		return CurlCode.fromValue(curlEasySetoptLongNative(handle,
				opt.getValue(), value));
	}

	private native int curlEasySetoptLongNative(long handle, int opt, long value);

	public CurlCode curlEasySetopt(OptFunctionPoint opt, WriteCallback callback) {
		// Log.v(TAG, "curlEastSetopt: " + opt + "=" + callback);
		return CurlCode.fromValue(curlEasySetoptFunctionNative(handle,
				opt.getValue(), callback));
	}

	private native int curlEasySetoptFunctionNative(long handle, int opt,
			Callback callback);

	public CurlCode curlEasySetopt(OptObjectPoint opt, String value) {
		// Log.v(TAG, "curlEastSetopt: " + opt + "=" + value);
		return CurlCode.fromValue(curlEasySetoptObjectPointNative(handle,
				opt.getValue(), value));
	}

	private native int curlEasySetoptObjectPointNative(long handle, int opt,
			String value);

	public CurlCode curlEasySetopt(OptObjectPoint opt, byte[] value) {
		// Log.v(TAG, "curlEastSetopt: " + opt + "=" + value);
		return CurlCode.fromValue(curlEasySetoptObjectPointBytesNative(handle,
				opt.getValue(), value));
	}

	private native int curlEasySetoptObjectPointBytesNative(long handle,
			int opt, byte[] value);

	public CurlCode curlEasySetopt(OptObjectPoint opt, String[] values) {
		// Log.v(TAG, "curlEastSetopt: " + opt + "=" + values);
		return CurlCode.fromValue(curlEasySetoptObjectPointArrayNative(handle,
				opt.getValue(), values));
	}

	private native int curlEasySetoptObjectPointArrayNative(long handle,
			int opt, String[] value);

    /**
	 * 
	 * @param multiParts
	 * @return
	 */
	public CurlFormadd setFormdata(List<MultiPart> multiParts) {
		if (multiParts != null && multiParts.size() > 0) {
			return CurlFormadd.fromValue(setFormdataNative(handle,
					multiParts.toArray(new MultiPart[multiParts.size()])));
		} else {
			return CurlFormadd.CURL_FORMADD_NULL;
		}
	}

	private native int setFormdataNative(long handle, MultiPart[] multiArray);

	public CurlCode curlEasyPerform() {
		return CurlCode.fromValue(curlEasyPerformNavite(handle));
	}

	private native int curlEasyPerformNavite(long handle);
}	