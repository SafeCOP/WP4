/**
 * This class is automatically generated by mig. DO NOT EDIT THIS FILE.
 * This class implements a Java interface to the 'AgillaTraceMsg'
 * message type.
 */

package agilla.messages;

public class AgillaTraceMsg extends net.tinyos.message.Message {

    /** The default size of this message type in bytes. */
    public static final int DEFAULT_MESSAGE_SIZE = 22;

    /** The Active Message type associated with this message. */
    public static final int AM_TYPE = 147;

    /** Create a new AgillaTraceMsg of size 22. */
    public AgillaTraceMsg() {
        super(DEFAULT_MESSAGE_SIZE);
        amTypeSet(AM_TYPE);
    }

    /** Create a new AgillaTraceMsg of the given data_length. */
    public AgillaTraceMsg(int data_length) {
        super(data_length);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaTraceMsg with the given data_length
     * and base offset.
     */
    public AgillaTraceMsg(int data_length, int base_offset) {
        super(data_length, base_offset);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaTraceMsg using the given byte array
     * as backing store.
     */
    public AgillaTraceMsg(byte[] data) {
        super(data);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaTraceMsg using the given byte array
     * as backing store, with the given base offset.
     */
    public AgillaTraceMsg(byte[] data, int base_offset) {
        super(data, base_offset);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaTraceMsg using the given byte array
     * as backing store, with the given base offset and data length.
     */
    public AgillaTraceMsg(byte[] data, int base_offset, int data_length) {
        super(data, base_offset, data_length);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaTraceMsg embedded in the given message
     * at the given base offset.
     */
    public AgillaTraceMsg(net.tinyos.message.Message msg, int base_offset) {
        super(msg, base_offset, DEFAULT_MESSAGE_SIZE);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaTraceMsg embedded in the given message
     * at the given base offset and length.
     */
    public AgillaTraceMsg(net.tinyos.message.Message msg, int base_offset, int data_length) {
        super(msg, base_offset, data_length);
        amTypeSet(AM_TYPE);
    }

    /**
    /* Return a String representation of this message. Includes the
     * message type name and the non-indexed field values.
     */
    public String toString() {
      String s = "Message <AgillaTraceMsg> \n";
      try {
        s += "  [timestamp.high32=0x"+Long.toHexString(get_timestamp_high32())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [timestamp.low32=0x"+Long.toHexString(get_timestamp_low32())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [agentID=0x"+Long.toHexString(get_agentID())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [nodeID=0x"+Long.toHexString(get_nodeID())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [action=0x"+Long.toHexString(get_action())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [qid=0x"+Long.toHexString(get_qid())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [success=0x"+Long.toHexString(get_success())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [loc.x=0x"+Long.toHexString(get_loc_x())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [loc.y=0x"+Long.toHexString(get_loc_y())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      return s;
    }

    // Message-type-specific access methods appear below.

    /////////////////////////////////////////////////////////
    // Accessor methods for field: timestamp.high32
    //   Field type: long
    //   Offset (bits): 0
    //   Size (bits): 32
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'timestamp.high32' is signed (false).
     */
    public static boolean isSigned_timestamp_high32() {
        return false;
    }

    /**
     * Return whether the field 'timestamp.high32' is an array (false).
     */
    public static boolean isArray_timestamp_high32() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'timestamp.high32'
     */
    public static int offset_timestamp_high32() {
        return (0 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'timestamp.high32'
     */
    public static int offsetBits_timestamp_high32() {
        return 0;
    }

    /**
     * Return the value (as a long) of the field 'timestamp.high32'
     */
    public long get_timestamp_high32() {
        return (long)getUIntBEElement(offsetBits_timestamp_high32(), 32);
    }

    /**
     * Set the value of the field 'timestamp.high32'
     */
    public void set_timestamp_high32(long value) {
        setUIntBEElement(offsetBits_timestamp_high32(), 32, value);
    }

    /**
     * Return the size, in bytes, of the field 'timestamp.high32'
     */
    public static int size_timestamp_high32() {
        return (32 / 8);
    }

    /**
     * Return the size, in bits, of the field 'timestamp.high32'
     */
    public static int sizeBits_timestamp_high32() {
        return 32;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: timestamp.low32
    //   Field type: long
    //   Offset (bits): 32
    //   Size (bits): 32
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'timestamp.low32' is signed (false).
     */
    public static boolean isSigned_timestamp_low32() {
        return false;
    }

    /**
     * Return whether the field 'timestamp.low32' is an array (false).
     */
    public static boolean isArray_timestamp_low32() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'timestamp.low32'
     */
    public static int offset_timestamp_low32() {
        return (32 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'timestamp.low32'
     */
    public static int offsetBits_timestamp_low32() {
        return 32;
    }

    /**
     * Return the value (as a long) of the field 'timestamp.low32'
     */
    public long get_timestamp_low32() {
        return (long)getUIntBEElement(offsetBits_timestamp_low32(), 32);
    }

    /**
     * Set the value of the field 'timestamp.low32'
     */
    public void set_timestamp_low32(long value) {
        setUIntBEElement(offsetBits_timestamp_low32(), 32, value);
    }

    /**
     * Return the size, in bytes, of the field 'timestamp.low32'
     */
    public static int size_timestamp_low32() {
        return (32 / 8);
    }

    /**
     * Return the size, in bits, of the field 'timestamp.low32'
     */
    public static int sizeBits_timestamp_low32() {
        return 32;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: agentID
    //   Field type: int
    //   Offset (bits): 64
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'agentID' is signed (false).
     */
    public static boolean isSigned_agentID() {
        return false;
    }

    /**
     * Return whether the field 'agentID' is an array (false).
     */
    public static boolean isArray_agentID() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'agentID'
     */
    public static int offset_agentID() {
        return (64 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'agentID'
     */
    public static int offsetBits_agentID() {
        return 64;
    }

    /**
     * Return the value (as a int) of the field 'agentID'
     */
    public int get_agentID() {
        return (int)getUIntElement(offsetBits_agentID(), 16);
    }

    /**
     * Set the value of the field 'agentID'
     */
    public void set_agentID(int value) {
        setUIntElement(offsetBits_agentID(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'agentID'
     */
    public static int size_agentID() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'agentID'
     */
    public static int sizeBits_agentID() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: nodeID
    //   Field type: int
    //   Offset (bits): 80
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'nodeID' is signed (false).
     */
    public static boolean isSigned_nodeID() {
        return false;
    }

    /**
     * Return whether the field 'nodeID' is an array (false).
     */
    public static boolean isArray_nodeID() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'nodeID'
     */
    public static int offset_nodeID() {
        return (80 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'nodeID'
     */
    public static int offsetBits_nodeID() {
        return 80;
    }

    /**
     * Return the value (as a int) of the field 'nodeID'
     */
    public int get_nodeID() {
        return (int)getUIntElement(offsetBits_nodeID(), 16);
    }

    /**
     * Set the value of the field 'nodeID'
     */
    public void set_nodeID(int value) {
        setUIntElement(offsetBits_nodeID(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'nodeID'
     */
    public static int size_nodeID() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'nodeID'
     */
    public static int sizeBits_nodeID() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: action
    //   Field type: int
    //   Offset (bits): 96
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'action' is signed (false).
     */
    public static boolean isSigned_action() {
        return false;
    }

    /**
     * Return whether the field 'action' is an array (false).
     */
    public static boolean isArray_action() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'action'
     */
    public static int offset_action() {
        return (96 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'action'
     */
    public static int offsetBits_action() {
        return 96;
    }

    /**
     * Return the value (as a int) of the field 'action'
     */
    public int get_action() {
        return (int)getUIntElement(offsetBits_action(), 16);
    }

    /**
     * Set the value of the field 'action'
     */
    public void set_action(int value) {
        setUIntElement(offsetBits_action(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'action'
     */
    public static int size_action() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'action'
     */
    public static int sizeBits_action() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: qid
    //   Field type: int
    //   Offset (bits): 112
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'qid' is signed (false).
     */
    public static boolean isSigned_qid() {
        return false;
    }

    /**
     * Return whether the field 'qid' is an array (false).
     */
    public static boolean isArray_qid() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'qid'
     */
    public static int offset_qid() {
        return (112 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'qid'
     */
    public static int offsetBits_qid() {
        return 112;
    }

    /**
     * Return the value (as a int) of the field 'qid'
     */
    public int get_qid() {
        return (int)getUIntElement(offsetBits_qid(), 16);
    }

    /**
     * Set the value of the field 'qid'
     */
    public void set_qid(int value) {
        setUIntElement(offsetBits_qid(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'qid'
     */
    public static int size_qid() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'qid'
     */
    public static int sizeBits_qid() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: success
    //   Field type: int
    //   Offset (bits): 128
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'success' is signed (false).
     */
    public static boolean isSigned_success() {
        return false;
    }

    /**
     * Return whether the field 'success' is an array (false).
     */
    public static boolean isArray_success() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'success'
     */
    public static int offset_success() {
        return (128 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'success'
     */
    public static int offsetBits_success() {
        return 128;
    }

    /**
     * Return the value (as a int) of the field 'success'
     */
    public int get_success() {
        return (int)getUIntElement(offsetBits_success(), 16);
    }

    /**
     * Set the value of the field 'success'
     */
    public void set_success(int value) {
        setUIntElement(offsetBits_success(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'success'
     */
    public static int size_success() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'success'
     */
    public static int sizeBits_success() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: loc.x
    //   Field type: int
    //   Offset (bits): 144
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'loc.x' is signed (false).
     */
    public static boolean isSigned_loc_x() {
        return false;
    }

    /**
     * Return whether the field 'loc.x' is an array (false).
     */
    public static boolean isArray_loc_x() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'loc.x'
     */
    public static int offset_loc_x() {
        return (144 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'loc.x'
     */
    public static int offsetBits_loc_x() {
        return 144;
    }

    /**
     * Return the value (as a int) of the field 'loc.x'
     */
    public int get_loc_x() {
        return (int)getUIntBEElement(offsetBits_loc_x(), 16);
    }

    /**
     * Set the value of the field 'loc.x'
     */
    public void set_loc_x(int value) {
        setUIntBEElement(offsetBits_loc_x(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'loc.x'
     */
    public static int size_loc_x() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'loc.x'
     */
    public static int sizeBits_loc_x() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: loc.y
    //   Field type: int
    //   Offset (bits): 160
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'loc.y' is signed (false).
     */
    public static boolean isSigned_loc_y() {
        return false;
    }

    /**
     * Return whether the field 'loc.y' is an array (false).
     */
    public static boolean isArray_loc_y() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'loc.y'
     */
    public static int offset_loc_y() {
        return (160 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'loc.y'
     */
    public static int offsetBits_loc_y() {
        return 160;
    }

    /**
     * Return the value (as a int) of the field 'loc.y'
     */
    public int get_loc_y() {
        return (int)getUIntBEElement(offsetBits_loc_y(), 16);
    }

    /**
     * Set the value of the field 'loc.y'
     */
    public void set_loc_y(int value) {
        setUIntBEElement(offsetBits_loc_y(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'loc.y'
     */
    public static int size_loc_y() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'loc.y'
     */
    public static int sizeBits_loc_y() {
        return 16;
    }

}
