/**
 * This class is automatically generated by mig. DO NOT EDIT THIS FILE.
 * This class implements a Java interface to the 'AgillaAddressAckMsg'
 * message type.
 */

package agilla.messages;

public class AgillaAddressAckMsg extends net.tinyos.message.Message {

    /** The default size of this message type in bytes. */
    public static final int DEFAULT_MESSAGE_SIZE = 6;

    /** The Active Message type associated with this message. */
    public static final int AM_TYPE = 65;

    /** Create a new AgillaAddressAckMsg of size 6. */
    public AgillaAddressAckMsg() {
        super(DEFAULT_MESSAGE_SIZE);
        amTypeSet(AM_TYPE);
    }

    /** Create a new AgillaAddressAckMsg of the given data_length. */
    public AgillaAddressAckMsg(int data_length) {
        super(data_length);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaAddressAckMsg with the given data_length
     * and base offset.
     */
    public AgillaAddressAckMsg(int data_length, int base_offset) {
        super(data_length, base_offset);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaAddressAckMsg using the given byte array
     * as backing store.
     */
    public AgillaAddressAckMsg(byte[] data) {
        super(data);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaAddressAckMsg using the given byte array
     * as backing store, with the given base offset.
     */
    public AgillaAddressAckMsg(byte[] data, int base_offset) {
        super(data, base_offset);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaAddressAckMsg using the given byte array
     * as backing store, with the given base offset and data length.
     */
    public AgillaAddressAckMsg(byte[] data, int base_offset, int data_length) {
        super(data, base_offset, data_length);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaAddressAckMsg embedded in the given message
     * at the given base offset.
     */
    public AgillaAddressAckMsg(net.tinyos.message.Message msg, int base_offset) {
        super(msg, base_offset, DEFAULT_MESSAGE_SIZE);
        amTypeSet(AM_TYPE);
    }

    /**
     * Create a new AgillaAddressAckMsg embedded in the given message
     * at the given base offset and length.
     */
    public AgillaAddressAckMsg(net.tinyos.message.Message msg, int base_offset, int data_length) {
        super(msg, base_offset, data_length);
        amTypeSet(AM_TYPE);
    }

    /**
    /* Return a String representation of this message. Includes the
     * message type name and the non-indexed field values.
     */
    public String toString() {
      String s = "Message <AgillaAddressAckMsg> \n";
      try {
        s += "  [success=0x"+Long.toHexString(get_success())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [oldAddr=0x"+Long.toHexString(get_oldAddr())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      try {
        s += "  [newAddr=0x"+Long.toHexString(get_newAddr())+"]\n";
      } catch (ArrayIndexOutOfBoundsException aioobe) { /* Skip field */ }
      return s;
    }

    // Message-type-specific access methods appear below.

    /////////////////////////////////////////////////////////
    // Accessor methods for field: success
    //   Field type: int, unsigned
    //   Offset (bits): 0
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
        return (0 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'success'
     */
    public static int offsetBits_success() {
        return 0;
    }

    /**
     * Return the value (as a int) of the field 'success'
     */
    public int get_success() {
        return (int)getUIntBEElement(offsetBits_success(), 16);
    }

    /**
     * Set the value of the field 'success'
     */
    public void set_success(int value) {
        setUIntBEElement(offsetBits_success(), 16, value);
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
    // Accessor methods for field: oldAddr
    //   Field type: int, unsigned
    //   Offset (bits): 16
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'oldAddr' is signed (false).
     */
    public static boolean isSigned_oldAddr() {
        return false;
    }

    /**
     * Return whether the field 'oldAddr' is an array (false).
     */
    public static boolean isArray_oldAddr() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'oldAddr'
     */
    public static int offset_oldAddr() {
        return (16 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'oldAddr'
     */
    public static int offsetBits_oldAddr() {
        return 16;
    }

    /**
     * Return the value (as a int) of the field 'oldAddr'
     */
    public int get_oldAddr() {
        return (int)getUIntBEElement(offsetBits_oldAddr(), 16);
    }

    /**
     * Set the value of the field 'oldAddr'
     */
    public void set_oldAddr(int value) {
        setUIntBEElement(offsetBits_oldAddr(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'oldAddr'
     */
    public static int size_oldAddr() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'oldAddr'
     */
    public static int sizeBits_oldAddr() {
        return 16;
    }

    /////////////////////////////////////////////////////////
    // Accessor methods for field: newAddr
    //   Field type: int, unsigned
    //   Offset (bits): 32
    //   Size (bits): 16
    /////////////////////////////////////////////////////////

    /**
     * Return whether the field 'newAddr' is signed (false).
     */
    public static boolean isSigned_newAddr() {
        return false;
    }

    /**
     * Return whether the field 'newAddr' is an array (false).
     */
    public static boolean isArray_newAddr() {
        return false;
    }

    /**
     * Return the offset (in bytes) of the field 'newAddr'
     */
    public static int offset_newAddr() {
        return (32 / 8);
    }

    /**
     * Return the offset (in bits) of the field 'newAddr'
     */
    public static int offsetBits_newAddr() {
        return 32;
    }

    /**
     * Return the value (as a int) of the field 'newAddr'
     */
    public int get_newAddr() {
        return (int)getUIntBEElement(offsetBits_newAddr(), 16);
    }

    /**
     * Set the value of the field 'newAddr'
     */
    public void set_newAddr(int value) {
        setUIntBEElement(offsetBits_newAddr(), 16, value);
    }

    /**
     * Return the size, in bytes, of the field 'newAddr'
     */
    public static int size_newAddr() {
        return (16 / 8);
    }

    /**
     * Return the size, in bits, of the field 'newAddr'
     */
    public static int sizeBits_newAddr() {
        return 16;
    }

}
