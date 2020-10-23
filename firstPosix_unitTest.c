/* Some of the mock function definitions and unit test function to test the function createFile()
   Please note the below test cases had been written considering cmockery framework.
*/

ssize_t mock_read(int fd, void *buf, size_t count)
{
    (void) fd;
    (void) buf;
    (void) count;
	
    return (ssize_t) mock();
}

ssize_t mock_write(int fd, const void *buf, size_t count)
{
	(void) fd;
    (void) buf;
    (void) count;
	
    return (ssize_t) mock();
}

int mock_close(int fd)
{
	(void) fd;
	return (int) mock();
}

int mock_open(const char *pathName, int flags, mode_t mode)
{
   (void) pathName;
   (void) flags;
   (void) mode;
	
    return (int) mock();
}

static void test_createFile(void **state __attribute((unused)))
{
    char ch[1024];
    char *fileName = "output.txt";
    int retVal;
    /* Test for unhappy paths */
   
    //NULL filename
    retVal = createFile(NULL);
    assert_int_equal(retVal, -1);

    // open failed
    will_return(mock_open, -1);
    retVal = createFile(fileName);
    assert_int_equal(retVal, -1);
    
    // read failed
    will_return(mock_open, 0);
    will_return(mock_read, -1);
    will_return(mock_close, 0);
    retVal = createFile(fileName);
    assert_int_equal(retVal, -1);

    // write failed
    will_return(mock_open, 0);
    will_return(mock_read, 100);
    will_return(mock_write, -1);
    will_return(mock_close, 0);
    retVal = createFile(fileName);
    assert_int_equal(retVal, -1);

    // close failed
    will_return(mock_open, 0);
    will_return(mock_read, 100);
    will_return(mock_write, 100);
    will_return(mock_close, -1);
    will_return(mock_read, 0);

    retVal = createFile(fileName);
    assert_int_equal(retVal, -1);
    /* happy path */

    will_return(mock_open, 0);
    will_return(mock_read, 100);
    will_return(mock_write, 100);
    will_return(mock_close, 0);
    will_return(mock_read, 0);

    retVal = createFile(fileName);
    assert_int_equal(retVal, 0);

}
