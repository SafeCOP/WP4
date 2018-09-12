/**
 * This class is automatically generated by mig. DO NOT EDIT THIS FILE.
 * This class implements a Java interface to the 'AgillaQueryReplyAllAgentsMsg'
 * message type.
 */

package agilla.messages;

public class AgillaQueryReplyAllAgentsMsg extends net.tinyos.message.Message {

    /** The default size of this message type in bytes. */
    public static final int DEFAULT_MESSAGE_SIZE = 24;

    /** The Active Message type associated with this message. */
    public static final int AM_TYPE = 74;

    /** Create a new AgillaQueryReplyAllAgentsMsg of size 24. */
    public AgillaQueryReplyAllAgentsMsg() {
        super(DEFAULT_MESSAGE_SIZE);
        amTypeSet(AM_TYPE);
    }

    /** Create a new AgillaQueryReplyAllAgentsMsg of the given data_length. */
    public AgillaQueryReplyAllAgentsMsg(int data_length) {
        super(data_length);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaQueryReplyAllAgentsMsg with the given data_length
     * and base offset.
     */
    public AgillaQueryReplyAllAgentsMsg(int data_length, int base_offset) {
        super(data_length, base_offset);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaQueryReplyAllAgentsMsg using the given byte array
     * as backing store.
     */
    public AgillaQueryReplyAllAgentsMsg(byte[] data) {
        super(data);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaQueryReplyAllAgentsMsg using the given byte array
     * as backing store, with the given base offset.
     */
    public AgillaQueryReplyAllAgentsMsg(byte[] data, int base_offset) {
        super(data, base_offset);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaQueryReplyAllAgentsMsg using the given byte array
     * as backing store, with the given base offset and data length.
     */
    public AgillaQueryReplyAllAgentsMsg(byte[] data, int base_offset, int data_length) {
        super(data, base_offset, data_length);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaQueryReplyAllAgentsMsg embedded in the given message
     * at the given base offset.
     */
    public AgillaQueryReplyAllAgentsMsg(net.tinyos.message.Message msg, int base_offset) {
        super(msg, base_offset, DEFAULT_MESSAGE_SIZE);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaQueryReplyAllAgentsMsg embedded in the given message
     * at the given base offset and length.
     */
    public AgillaQueryReplyAllAgentsMsg(net.tinyos.message.Message msg, int base_offset, int data_length) {
        super(msg, base_offset, data_length);
        amTypeSet(AM_TYPE);
    }

    /**
    /* Return a String representation of this message. Includes the
     * message type name and the non-indexed field values.
     */
    public String toString() {
      String s = "Message <AgillaQueryReplyAllAgentsMsg> \n";
      try {
        s += "  [agent_id.id=0x"+Long.toHexString(get_agent_id_id())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [src=0x"+Long.toHexString(get_src())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [dest=0x"+Long.toHexString(get_dest())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [qid=0x"+Long.toHexString(get_qid())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [num_agents=0x"+Long.toHexString(get_num_agents())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [flags=0x"+Long.toHexString(get_flags())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [agent_info.agent_id.id=";
        for (int i = 0; i < 2; i++) {
          s += "0x"+Long.toHexString(getElement_agent_info_agent_id_id(i) & 0xffff)+" ";
        }
        s += "]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [agent_info.loc.x=";
        for (int i = 0; i < 2; i++) {
          s += "0x"+Long.toHexString(getElement_agent_info_loc_x(i) & 0xffff)+" ";
        }
        s += "]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [agent_info.loc.y=";
        for (int i = 0; i < 2; i++) {
          s += "0x"+Long.toHexString(getElement_agent_info_loc_y(i) & 0xffff)+" ";
        }
        s += "]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      return s;
    }

    // Message-type-specific access methods appear below.

    /////////////////////////////////////////////////////////
    // Accessor methods for field: agent_id.id
    //   Field type: int
    //   Offset (bits): 0
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'agent_id.id' is signed (false).
     */
    public static boolean isSigned_agent_id_id() {
        return false;
    }

    /**
     * Return whether the field 'agent_id.id' is an array (false).
     */
    public static boolean isArray_agent_id_id() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'agent_id.id'
     */
    public static int offset_agent_id_id() {
        return (0 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'agent_id.id'
     */
    public static int offsetBits_agent_id_id() {
        return 0;
    }

    /**
     * Return the value (as a int) of the field 'agent_id.id'
     */
    public int get_agent_id_id() {
        return (int)getUIntBEElement(offsetBits_agent_id_id(), 16);
    }

    /**
     * Set the value of the field 'agent_id.id'
     */
    public void set_agent_id_id(int value) {
        setUIntBEElement(offsetBits_agent_id_id(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'agent_id.id'
     */
    public static int size_agent_id_id() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'agent_id.id'
     */
    public static int sizeBits_agent_id_id() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: src
    //   Field type: int
    //   Offset (bits): 16
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'src' is signed (false).
     */
    public static boolean isSigned_src() {
        return false;
    }

    /**
     * Return whether the field 'src' is an array (false).
     */
    public static boolean isArray_src() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'src'
     */
    public static int offset_src() {
        return (16 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'src'
     */
    public static int offsetBits_src() {
        return 16;
    }

    /**
     * Return the value (as a int) of the field 'src'
     */
    public int get_src() {
        return (int)getUIntBEElement(offsetBits_src(), 16);
    }

    /**
     * Set the value of the field 'src'
     */
    public void set_src(int value) {
        setUIntBEElement(offsetBits_src(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'src'
     */
    public static int size_src() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'src'
     */
    public static int sizeBits_src() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: dest
    //   Field type: int
    //   Offset (bits): 32
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'dest' is signed (false).
     */
    public static boolean isSigned_dest() {
        return false;
    }

    /**
     * Return whether the field 'dest' is an array (false).
     */
    public static boolean isArray_dest() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'dest'
     */
    public static int offset_dest() {
        return (32 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'dest'
     */
    public static int offsetBits_dest() {
        return 32;
    }

    /**
     * Return the value (as a int) of the field 'dest'
     */
    public int get_dest() {
        return (int)getUIntBEElement(offsetBits_dest(), 16);
    }

    /**
     * Set the value of the field 'dest'
     */
    public void set_dest(int value) {
        setUIntBEElement(offsetBits_dest(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'dest'
     */
    public static int size_dest() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'dest'
     */
    public static int sizeBits_dest() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: qid
    //   Field type: int
    //   Offset (bits): 48
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
        return (48 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'qid'
     */
    public static int offsetBits_qid() {
        return 48;
    }

    /**
     * Return the value (as a int) of the field 'qid'
     */
    public int get_qid() {
        return (int)getUIntBEElement(offsetBits_qid(), 16);
    }

    /**
     * Set the value of the field 'qid'
     */
    public void set_qid(int value) {
        setUIntBEElement(offsetBits_qid(), 16, value);
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
    // Accessor methods for field: num_agents
    //   Field type: int
    //   Offset (bits): 64
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'num_agents' is signed (false).
     */
    public static boolean isSigned_num_agents() {
        return false;
    }

    /**
     * Return whether the field 'num_agents' is an array (false).
     */
    public static boolean isArray_num_agents() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'num_agents'
     */
    public static int offset_num_agents() {
        return (64 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'num_agents'
     */
    public static int offsetBits_num_agents() {
        return 64;
    }

    /**
     * Return the value (as a int) of the field 'num_agents'
     */
    public int get_num_agents() {
        return (int)getUIntBEElement(offsetBits_num_agents(), 16);
    }

    /**
     * Set the value of the field 'num_agents'
     */
    public void set_num_agents(int value) {
        setUIntBEElement(offsetBits_num_agents(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'num_agents'
     */
    public static int size_num_agents() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'num_agents'
     */
    public static int sizeBits_num_agents() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: flags
    //   Field type: int
    //   Offset (bits): 80
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'flags' is signed (false).
     */
    public static boolean isSigned_flags() {
        return false;
    }

    /**
     * Return whether the field 'flags' is an array (false).
     */
    public static boolean isArray_flags() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'flags'
     */
    public static int offset_flags() {
        return (80 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'flags'
     */
    public static int offsetBits_flags() {
        return 80;
    }

    /**
     * Return the value (as a int) of the field 'flags'
     */
    public int get_flags() {
        return (int)getUIntBEElement(offsetBits_flags(), 16);
    }

    /**
     * Set the value of the field 'flags'
     */
    public void set_flags(int value) {
        setUIntBEElement(offsetBits_flags(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'flags'
     */
    public static int size_flags() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'flags'
     */
    public static int sizeBits_flags() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: agent_info.agent_id.id
    //   Field type: int[]
    //   Offset (bits): 0
    //   Size of each element (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'agent_info.agent_id.id' is signed (false).
     */
    public static boolean isSigned_agent_info_agent_id_id() {
        return false;
    }

    /**
     * Return whether the field 'agent_info.agent_id.id' is an array (true).
     */
    public static boolean isArray_agent_info_agent_id_id() {
        return true;
    }

    /**
     * Return the offset (in bytes) of the field 'agent_info.agent_id.id'
     */
    public static int offset_agent_info_agent_id_id(int index1) {
        int offset = 0;
        if (index1 < 0 || index1 >= 2) throw new ArrayIndexOutOfBoundsException();
        offset += 96 + index1 * 48;
        return (offset / 8);
    }

    /**
     * Return the offset (in bits) of the field 'agent_info.agent_id.id'
     */
    public static int offsetBits_agent_info_agent_id_id(int index1) {
        int offset = 0;
        if (index1 < 0 || index1 >= 2) throw new ArrayIndexOutOfBoundsException();
        offset += 96 + index1 * 48;
        return offset;
    }

    /**
     * Return the entire array 'agent_info.agent_id.id' as a int[]
     */
    public int[] get_agent_info_agent_id_id() {
        int[] tmp = new int[2];
        for (int index0 = 0; index0 < numElements_agent_info_agent_id_id(0); index0++) {
            tmp[index0] = getElement_agent_info_agent_id_id(index0);
        }
        return tmp;
    }

    /**
     * Set the contents of the array 'agent_info.agent_id.id' from the given int[]
     */
    public void set_agent_info_agent_id_id(int[] value) {
        for (int index0 = 0; index0 < value.length; index0++) {
            setElement_agent_info_agent_id_id(index0, value[index0]);
        }
    }

    /**
     * Return an element (as a int) of the array 'agent_info.agent_id.id'
     */
    public int getElement_agent_info_agent_id_id(int index1) {
        return (int)getUIntBEElement(offsetBits_agent_info_agent_id_id(index1), 16);
    }

    /**
     * Set an element of the array 'agent_info.agent_id.id'
     */
    public void setElement_agent_info_agent_id_id(int index1, int value) {
        setUIntBEElement(offsetBits_agent_info_agent_id_id(index1), 16, value);
    }

    /**
     * Return the total size, in bytes, of the array 'agent_info.agent_id.id'
     */
    public static int totalSize_agent_info_agent_id_id() {
        return (96 / 8);
    }

    /**
     * Return the total size, in bits, of the array 'agent_info.agent_id.id'
     */
    public static int totalSizeBits_agent_info_agent_id_id() {
        return 96;
    }

    /**
     * Return the size, in bytes, of each element of the array 'agent_info.agent_id.id'
     */
    public static int elementSize_agent_info_agent_id_id() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of each element of the array 'agent_info.agent_id.id'
     */
    public static int elementSizeBits_agent_info_agent_id_id() {
        return 16;
    }

    /**
     * Return the number of dimensions in the array 'agent_info.agent_id.id'
     */
    public static int numDimensions_agent_info_agent_id_id() {
        return 1;
    }

    /**
     * Return the number of elements in the array 'agent_info.agent_id.id'
     */
    public static int numElements_agent_info_agent_id_id() {
        return 2;
    }

    /**
     * Return the number of elements in the array 'agent_info.agent_id.id'
     * for the given dimension.
     */
    public static int numElements_agent_info_agent_id_id(int dimension) {
      int array_dims[] = { 2,  };
        if (dimension < 0 || dimension >= 1) throw new ArrayIndexOutOfBoundsException();
        if (array_dims[dimension] == 0) throw new IllegalArgumentException("Array dimension "+dimension+" has unknown size");
        return array_dims[dimension];
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: agent_info.loc.x
    //   Field type: int[]
    //   Offset (bits): 16
    //   Size of each element (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'agent_info.loc.x' is signed (false).
     */
    public static boolean isSigned_agent_info_loc_x() {
        return false;
    }

    /**
     * Return whether the field 'agent_info.loc.x' is an array (true).
     */
    public static boolean isArray_agent_info_loc_x() {
        return true;
    }

    /**
     * Return the offset (in bytes) of the field 'agent_info.loc.x'
     */
    public static int offset_agent_info_loc_x(int index1) {
        int offset = 16;
        if (index1 < 0 || index1 >= 2) throw new ArrayIndexOutOfBoundsException();
        offset += 96 + index1 * 48;
        return (offset / 8);
    }

    /**
     * Return the offset (in bits) of the field 'agent_info.loc.x'
     */
    public static int offsetBits_agent_info_loc_x(int index1) {
        int offset = 16;
        if (index1 < 0 || index1 >= 2) throw new ArrayIndexOutOfBoundsException();
        offset += 96 + index1 * 48;
        return offset;
    }

    /**
     * Return the entire array 'agent_info.loc.x' as a int[]
     */
    public int[] get_agent_info_loc_x() {
        int[] tmp = new int[2];
        for (int index0 = 0; index0 < numElements_agent_info_loc_x(0); index0++) {
            tmp[index0] = getElement_agent_info_loc_x(index0);
        }
        return tmp;
    }

    /**
     * Set the contents of the array 'agent_info.loc.x' from the given int[]
     */
    public void set_agent_info_loc_x(int[] value) {
        for (int index0 = 0; index0 < value.length; index0++) {
            setElement_agent_info_loc_x(index0, value[index0]);
        }
    }

    /**
     * Return an element (as a int) of the array 'agent_info.loc.x'
     */
    public int getElement_agent_info_loc_x(int index1) {
        return (int)getUIntBEElement(offsetBits_agent_info_loc_x(index1), 16);
    }

    /**
     * Set an element of the array 'agent_info.loc.x'
     */
    public void setElement_agent_info_loc_x(int index1, int value) {
        setUIntBEElement(offsetBits_agent_info_loc_x(index1), 16, value);
    }

    /**
     * Return the total size, in bytes, of the array 'agent_info.loc.x'
     */
    public static int totalSize_agent_info_loc_x() {
        return (96 / 8);
    }

    /**
     * Return the total size, in bits, of the array 'agent_info.loc.x'
     */
    public static int totalSizeBits_agent_info_loc_x() {
        return 96;
    }

    /**
     * Return the size, in bytes, of each element of the array 'agent_info.loc.x'
     */
    public static int elementSize_agent_info_loc_x() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of each element of the array 'agent_info.loc.x'
     */
    public static int elementSizeBits_agent_info_loc_x() {
        return 16;
    }

    /**
     * Return the number of dimensions in the array 'agent_info.loc.x'
     */
    public static int numDimensions_agent_info_loc_x() {
        return 1;
    }

    /**
     * Return the number of elements in the array 'agent_info.loc.x'
     */
    public static int numElements_agent_info_loc_x() {
        return 2;
    }

    /**
     * Return the number of elements in the array 'agent_info.loc.x'
     * for the given dimension.
     */
    public static int numElements_agent_info_loc_x(int dimension) {
      int array_dims[] = { 2,  };
        if (dimension < 0 || dimension >= 1) throw new ArrayIndexOutOfBoundsException();
        if (array_dims[dimension] == 0) throw new IllegalArgumentException("Array dimension "+dimension+" has unknown size");
        return array_dims[dimension];
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: agent_info.loc.y
    //   Field type: int[]
    //   Offset (bits): 32
    //   Size of each element (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'agent_info.loc.y' is signed (false).
     */
    public static boolean isSigned_agent_info_loc_y() {
        return false;
    }

    /**
     * Return whether the field 'agent_info.loc.y' is an array (true).
     */
    public static boolean isArray_agent_info_loc_y() {
        return true;
    }

    /**
     * Return the offset (in bytes) of the field 'agent_info.loc.y'
     */
    public static int offset_agent_info_loc_y(int index1) {
        int offset = 32;
        if (index1 < 0 || index1 >= 2) throw new ArrayIndexOutOfBoundsException();
        offset += 96 + index1 * 48;
        return (offset / 8);
    }

    /**
     * Return the offset (in bits) of the field 'agent_info.loc.y'
     */
    public static int offsetBits_agent_info_loc_y(int index1) {
        int offset = 32;
        if (index1 < 0 || index1 >= 2) throw new ArrayIndexOutOfBoundsException();
        offset += 96 + index1 * 48;
        return offset;
    }

    /**
     * Return the entire array 'agent_info.loc.y' as a int[]
     */
    public int[] get_agent_info_loc_y() {
        int[] tmp = new int[2];
        for (int index0 = 0; index0 < numElements_agent_info_loc_y(0); index0++) {
            tmp[index0] = getElement_agent_info_loc_y(index0);
        }
        return tmp;
    }

    /**
     * Set the contents of the array 'agent_info.loc.y' from the given int[]
     */
    public void set_agent_info_loc_y(int[] value) {
        for (int index0 = 0; index0 < value.length; index0++) {
            setElement_agent_info_loc_y(index0, value[index0]);
        }
    }

    /**
     * Return an element (as a int) of the array 'agent_info.loc.y'
     */
    public int getElement_agent_info_loc_y(int index1) {
        return (int)getUIntBEElement(offsetBits_agent_info_loc_y(index1), 16);
    }

    /**
     * Set an element of the array 'agent_info.loc.y'
     */
    public void setElement_agent_info_loc_y(int index1, int value) {
        setUIntBEElement(offsetBits_agent_info_loc_y(index1), 16, value);
    }

    /**
     * Return the total size, in bytes, of the array 'agent_info.loc.y'
     */
    public static int totalSize_agent_info_loc_y() {
        return (96 / 8);
    }

    /**
     * Return the total size, in bits, of the array 'agent_info.loc.y'
     */
    public static int totalSizeBits_agent_info_loc_y() {
        return 96;
    }

    /**
     * Return the size, in bytes, of each element of the array 'agent_info.loc.y'
     */
    public static int elementSize_agent_info_loc_y() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of each element of the array 'agent_info.loc.y'
     */
    public static int elementSizeBits_agent_info_loc_y() {
        return 16;
    }

    /**
     * Return the number of dimensions in the array 'agent_info.loc.y'
     */
    public static int numDimensions_agent_info_loc_y() {
        return 1;
    }

    /**
     * Return the number of elements in the array 'agent_info.loc.y'
     */
    public static int numElements_agent_info_loc_y() {
        return 2;
    }

    /**
     * Return the number of elements in the array 'agent_info.loc.y'
     * for the given dimension.
     */
    public static int numElements_agent_info_loc_y(int dimension) {
      int array_dims[] = { 2,  };
        if (dimension < 0 || dimension >= 1) throw new ArrayIndexOutOfBoundsException();
        if (array_dims[dimension] == 0) throw new IllegalArgumentException("Array dimension "+dimension+" has unknown size");
        return array_dims[dimension];
    }

}