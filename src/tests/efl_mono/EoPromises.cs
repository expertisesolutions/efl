/*
 * Copyright 2019 by its authors. See AUTHORS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
using System;
using System.Threading.Tasks;
using System.Threading;

namespace TestSuite
{

class TestEoPromises
{
    public static void test_simple_task_run()
    {
        Efl.Loop loop = Efl.App.AppMain;
        Eina.Future future = loop.Idle();

        bool callbackCalled = false;
        int ret_code = 1992;

        future.Then((Eina.Value value) => {
            callbackCalled = true;
            Eina.Value v = new Eina.Value(Eina.ValueType.Int32);
            v.Set(ret_code);
            loop.Quit(v);
            return value;
        });
        Eina.Value ret_value = loop.Begin();

        Test.Assert(callbackCalled, "Future loop callback must have been called.");

        Test.AssertEquals(ret_value.GetValueType(), Eina.ValueType.Int32);

        int ret_from_value;
        Test.Assert(ret_value.Get(out ret_from_value));
        Test.AssertEquals(ret_from_value, ret_code);

    }

    public static void test_object_promise()
    {
        Efl.Loop loop = Efl.App.AppMain;
        var obj = new Dummy.TestObject();

        Eina.Future future = obj.GetFuture();

        bool callbackCalled = false;
        int receivedValue = -1;
        int sentValue = 1984;
        future.Then((Eina.Value value) => {
            callbackCalled = true;
            Test.AssertEquals(value.GetValueType(), Eina.ValueType.Int32);
            value.Get(out receivedValue);

            return value;
        });

        obj.FulfillPromise(sentValue);

        loop.Iterate();
        Test.Assert(callbackCalled, "Future callback must have been called.");
        Test.AssertEquals(receivedValue, sentValue);
        obj.Dispose();
    }

    public static void test_object_promise_cancel()
    {
        Efl.Loop loop = Efl.App.AppMain;
        var obj = new Dummy.TestObject();

        Eina.Future future = obj.GetFuture();

        bool callbackCalled = false;
        Eina.Error receivedError = -1;
        Eina.Error sentError = 120;
        future.Then((Eina.Value value) => {
            callbackCalled = true;
            Test.AssertEquals(value.GetValueType(), Eina.ValueType.Error);
            value.Get(out receivedError);

            return value;
        });

        obj.RejectPromise(sentError);

        loop.Iterate();
        Test.Assert(callbackCalled, "Future callback must have been called.");
        Test.AssertEquals(receivedError, sentError);
        obj.Dispose();
    }

}

class LoopConsumer
{
    public static async Task Consume(Efl.Loop loop)
    {
        Task<Eina.Value> task = loop.IdleAsync();
        Eina.Value v = await task.ConfigureAwait(false);
        loop.Quit(v);
    }
}

class TestLoopEoAsyncMethods
{
    public static void test_simple_async()
    {
        Efl.Loop loop = Efl.App.AppMain;
        Task t = LoopConsumer.Consume(loop);

        loop.Begin();
        Test.Assert(t.Wait(1000), "Task should have been completed in time.");
    }
}

class TestEoAsyncMethods
{

    public static void test_async_fulfill()
    {
        Efl.Loop loop = Efl.App.AppMain;
        var obj = new Dummy.TestObject();

        Task<Eina.Value> task = obj.GetFutureAsync();

        int sentValue = 1337;

        obj.FulfillPromise(sentValue);
        loop.Iterate();

        Eina.Value v = task.Result;
        Test.AssertEquals(v.GetValueType(), Eina.ValueType.Int32);

        int receivedValue;
        v.Get(out receivedValue);
        Test.AssertEquals(receivedValue, sentValue);
        obj.Dispose();
    }

    public static void test_async_cancel()
    {
        Efl.Loop loop = Efl.App.AppMain;
        var obj = new Dummy.TestObject();

        CancellationTokenSource cancelSrc = new CancellationTokenSource();
        Task<Eina.Value> task = obj.GetFutureAsync(cancelSrc.Token);

        cancelSrc.Cancel();
        loop.Iterate();

        bool raised = false;
        try
        {
            Eina.Value v = task.Result;
        }
        catch (AggregateException ae)
        {
            raised = true;
            ae.Handle((x) =>
            {
                System.Console.WriteLine("Exception is {0}", x.GetType().Name);
                Test.Assert(x is TaskCanceledException, "AggregateException must have been TaskCanceledException");
                return true;
            });
        }

        Test.Assert(raised, "AggregateException must have been raised.");
        cancelSrc.Dispose();
        obj.Dispose();
    }

    public static void test_async_reject()
    {
        Efl.Loop loop = Efl.App.AppMain;
        var obj = new Dummy.TestObject();

        Task<Eina.Value> task = obj.GetFutureAsync();

        Eina.Error sentError = 1337;
        obj.RejectPromise(sentError);

        loop.Iterate();

        bool raised = false;
        try
        {
            Eina.Value v = task.Result;
        }
        catch (AggregateException ae)
        {
            raised = true;
            ae.Handle((x) =>
            {
                Test.Assert(x is Efl.FutureException, "AggregateException must have been TaskCanceledException");
                Efl.FutureException ex = x as Efl.FutureException;
                Test.AssertEquals(ex.Error, sentError);
                return true;
            });
        }

        Test.Assert(raised, "AggregateException must have been raised.");
        obj.Dispose();
    }
}
}
