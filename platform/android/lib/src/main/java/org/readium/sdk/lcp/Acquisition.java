package org.readium.sdk.lcp;

public class Acquisition {
    /**
     * Native Container Pointer.
     * DO NOT USE FROM JAVA SIDE!
     */
    private final long nativePtr;

    public interface Listener {
        void onAcquisitionStarted();
        void onAcquisitionEnded();
        void onAcquisitionProgressed(float progress);
    }

    private Acquisition(long nativePtr) {
        this.nativePtr = nativePtr;
    }

    public void cancel() {

    }

    public void start(Listener listener) {
        this.nativeStart(listener, this.nativePtr);
    }

    private native void nativeStart(Listener listener, long nativePtr);
}